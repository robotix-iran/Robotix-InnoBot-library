/*!
 * @file Adafruit_NeoPixel.cpp
 *
 * @mainpage Arduino Library for driving Adafruit NeoPixel addressable LEDs,
 * FLORA RGB Smart Pixels and compatible devicess -- WS2811, WS2812, WS2812B,
 * SK6812, etc.
 *
 * @section intro_sec Introduction
 *
 * This is the documentation for Adafruit's NeoPixel library for the
 * Arduino platform, allowing a broad range of microcontroller boards
 * (most AVR boards, many ARM devices, ESP8266 and ESP32, among others)
 * to control Adafruit NeoPixels, FLORA RGB Smart Pixels and compatible
 * devices -- WS2811, WS2812, WS2812B, SK6812, etc.
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing products
 * from Adafruit!
 *
 * @section author Author
 *
 * Written by Phil "Paint Your Dragon" Burgess for Adafruit Industries,
 * with contributions by PJRC, Michael Miller and other members of the
 * open source community.
 *
 * @section license License
 *
 * This file is part of the Adafruit_NeoPixel library.
 *
 * Adafruit_NeoPixel is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Adafruit_NeoPixel is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with NeoPixel. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 */

 #include "rgb.h"

 #if defined(TARGET_LPC1768)
 #include <time.h>
 #endif
 
 #if defined(NRF52) || defined(NRF52_SERIES)
 #include "nrf.h"
 
 // Interrupt is only disabled if there is no PWM device available
 // Note: Adafruit Bluefruit nrf52 does not use this option
 //#define NRF52_DISABLE_INT
 #endif
 
 #if defined(ARDUINO_ARCH_NRF52840)
 #if defined __has_include
 #if __has_include(<pinDefinitions.h>)
 #include <pinDefinitions.h>
 #endif
 #endif
 #endif
 
 /*!
   @brief   NeoPixel constructor when length, pin and pixel type are known
            at compile-time.
   @param   n  Number of NeoPixels in strand.
   @param   p  Arduino pin number which will drive the NeoPixel data in.
   @param   t  Pixel type -- add together NEO_* constants defined in
               Adafruit_NeoPixel.h, for example NEO_GRB+NEO_KHZ800 for
               NeoPixels expecting an 800 KHz (vs 400 KHz) data stream
               with color bytes expressed in green, red, blue order per
               pixel.
   @return  Adafruit_NeoPixel object. Call the begin() function before use.
 */
 Adafruit_NeoPixel::Adafruit_NeoPixel(uint16_t n, int16_t p, neoPixelType t)
     : begun(false), brightness(0), pixels(NULL), endTime(0) {
   updateType(t);
   updateLength(n);
   setPin(p);
 #if defined(ARDUINO_ARCH_RP2040)
   // Find a free SM on one of the PIO's
   sm = pio_claim_unused_sm(pio, false); // don't panic
   // Try pio1 if SM not found
   if (sm < 0) {
     pio = pio1;
     sm = pio_claim_unused_sm(pio, true); // panic if no SM is free
   }
   init = true;
 #endif
 #if defined(ESP32)
 #if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0)
   espInit();
 #endif
 #endif
 }
 
 /*!
   @brief   "Empty" NeoPixel constructor when length, pin and/or pixel type
            are not known at compile-time, and must be initialized later with
            updateType(), updateLength() and setPin().
   @return  Adafruit_NeoPixel object. Call the begin() function before use.
   @note    This function is deprecated, here only for old projects that
            may still be calling it. New projects should instead use the
            'new' keyword with the first constructor syntax (length, pin,
            type).
 */
 Adafruit_NeoPixel::Adafruit_NeoPixel()
     :
 #if defined(NEO_KHZ400)
       is800KHz(true),
 #endif
       begun(false), numLEDs(0), numBytes(0), pin(-1), brightness(0),
       pixels(NULL), rOffset(1), gOffset(0), bOffset(2), wOffset(1), endTime(0) {
 }
 
 /*!
   @brief   Deallocate Adafruit_NeoPixel object, set data pin back to INPUT.
 */
 Adafruit_NeoPixel::~Adafruit_NeoPixel() {
   free(pixels);
   if (pin >= 0)
     pinMode(pin, INPUT);
 }
 
 /*!
   @brief   Configure NeoPixel pin for output.
 */
 void Adafruit_NeoPixel::begin(void) {
   if (pin >= 0) {
     pinMode(pin, OUTPUT);
     digitalWrite(pin, LOW);
   }
   begun = true;
 }
 
 /*!
   @brief   Change the length of a previously-declared Adafruit_NeoPixel
            strip object. Old data is deallocated and new data is cleared.
            Pin number and pixel format are unchanged.
   @param   n  New length of strip, in pixels.
   @note    This function is deprecated, here only for old projects that
            may still be calling it. New projects should instead use the
            'new' keyword with the first constructor syntax (length, pin,
            type).
 */
 void Adafruit_NeoPixel::updateLength(uint16_t n) {
   free(pixels); // Free existing data (if any)
 
   // Allocate new data -- note: ALL PIXELS ARE CLEARED
   numBytes = n * ((wOffset == rOffset) ? 3 : 4);
   if ((pixels = (uint8_t *)malloc(numBytes))) {
     memset(pixels, 0, numBytes);
     numLEDs = n;
   } else {
     numLEDs = numBytes = 0;
   }
 }
 
 /*!
   @brief   Change the pixel format of a previously-declared
            Adafruit_NeoPixel strip object. If format changes from one of
            the RGB variants to an RGBW variant (or RGBW to RGB), the old
            data will be deallocated and new data is cleared. Otherwise,
            the old data will remain in RAM and is not reordered to the
            new format, so it's advisable to follow up with clear().
   @param   t  Pixel type -- add together NEO_* constants defined in
               Adafruit_NeoPixel.h, for example NEO_GRB+NEO_KHZ800 for
               NeoPixels expecting an 800 KHz (vs 400 KHz) data stream
               with color bytes expressed in green, red, blue order per
               pixel.
   @note    This function is deprecated, here only for old projects that
            may still be calling it. New projects should instead use the
            'new' keyword with the first constructor syntax
            (length, pin, type).
 */
 void Adafruit_NeoPixel::updateType(neoPixelType t) {
   bool oldThreeBytesPerPixel = (wOffset == rOffset); // false if RGBW
 
   wOffset = (t >> 6) & 0b11; // See notes in header file
   rOffset = (t >> 4) & 0b11; // regarding R/G/B/W offsets
   gOffset = (t >> 2) & 0b11;
   bOffset = t & 0b11;
 #if defined(NEO_KHZ400)
   is800KHz = (t < 256); // 400 KHz flag is 1<<8
 #endif
 
   // If bytes-per-pixel has changed (and pixel data was previously
   // allocated), re-allocate to new size. Will clear any data.
   if (pixels) {
     bool newThreeBytesPerPixel = (wOffset == rOffset);
     if (newThreeBytesPerPixel != oldThreeBytesPerPixel)
       updateLength(numLEDs);
   }
 }
 
 // RP2040 specific driver
 #if defined(ARDUINO_ARCH_RP2040)
 void Adafruit_NeoPixel::rp2040Init(uint8_t pin, bool is800KHz)
 {
   uint offset = pio_add_program(pio, &ws2812_program);
 
   if (is800KHz)
   {
     // 800kHz, 8 bit transfers
     ws2812_program_init(pio, sm, offset, pin, 800000, 8);
   }
   else
   {
     // 400kHz, 8 bit transfers
     ws2812_program_init(pio, sm, offset, pin, 400000, 8);
   }
 }
 // Not a user API
 void  Adafruit_NeoPixel::rp2040Show(uint8_t pin, uint8_t *pixels, uint32_t numBytes, bool is800KHz)
 {
   if (this->init)
   {
     // On first pass through initialise the PIO
     rp2040Init(pin, is800KHz);
     this->init = false;
   }
 
   while(numBytes--)
     // Bits for transmission must be shifted to top 8 bits
     pio_sm_put_blocking(pio, sm, ((uint32_t)*pixels++)<< 24);
 }
 #elif defined(ARDUINO_ARCH_CH32)
 
 // F_CPU is defined to SystemCoreClock (not constant number)
 #if SYSCLK_FREQ_144MHz_HSE == 144000000 || SYSCLK_FREQ_HSE == 144000000 || \
   SYSCLK_FREQ_144MHz_HSI == 144000000 || SYSCLK_FREQ_HSI == 144000000
 #define CH32_F_CPU 144000000
 
 #elif SYSCLK_FREQ_120MHz_HSE == 120000000 || SYSCLK_FREQ_HSE == 120000000 || \
   SYSCLK_FREQ_120MHz_HSI == 120000000 || SYSCLK_FREQ_HSI == 120000000
 #define CH32_F_CPU 120000000
 
 #elif SYSCLK_FREQ_96MHz_HSE == 96000000 || SYSCLK_FREQ_HSE == 96000000 || \
   SYSCLK_FREQ_96MHz_HSI == 96000000 || SYSCLK_FREQ_HSI == 96000000
 #define CH32_F_CPU 96000000
 
 #elif SYSCLK_FREQ_72MHz_HSE == 72000000 || SYSCLK_FREQ_HSE == 72000000 || \
   SYSCLK_FREQ_72MHz_HSI == 72000000 || SYSCLK_FREQ_HSI == 72000000
 #define CH32_F_CPU 72000000
 
 #elif SYSCLK_FREQ_56MHz_HSE == 56000000 || SYSCLK_FREQ_HSE == 56000000 || \
   SYSCLK_FREQ_56MHz_HSI == 56000000 || SYSCLK_FREQ_HSI == 56000000
 #define CH32_F_CPU 56000000
 
 #elif SYSCLK_FREQ_48MHz_HSE == 48000000 || SYSCLK_FREQ_HSE == 48000000 || \
   SYSCLK_FREQ_48MHz_HSI == 48000000 || SYSCLK_FREQ_HSI == 48000000
 #define CH32_F_CPU 48000000
 
 #endif
 
 static void ch32Show(GPIO_TypeDef* ch_port, uint32_t ch_pin, uint8_t* pixels, uint32_t numBytes, bool is800KHz) {
   // not support 400khz
   if (!is800KHz) return;
 
   volatile uint32_t* set = &ch_port->BSHR;
   volatile uint32_t* clr = &ch_port->BCR;
 
   uint8_t* ptr = pixels;
   uint8_t* end = ptr + numBytes;
   uint8_t p = *ptr++;
   uint8_t bitMask = 0x80;
 
   // NVIC_DisableIRQ(SysTicK_IRQn);
 
   while (1) {
     if (p & bitMask) { // ONE
       // High 800ns
       *set = ch_pin;
       __asm volatile ("nop; nop; nop; nop; nop; nop; nop; nop;"
         "nop; nop; nop; nop; nop; nop; nop; nop;"
         "nop; nop; nop; nop; nop; nop; nop; nop;"
         "nop; nop; nop; nop; nop; nop; nop; nop;"
         "nop; nop; nop; nop; nop; nop; nop; nop;"
         "nop;"
 #if CH32_F_CPU >= 72000000
         "nop; nop; nop; nop; nop; nop; nop;"
         "nop; nop; nop; nop; nop; nop;"
 #endif
 #if CH32_F_CPU >= 96000000
         "nop; nop;"
         "nop; nop; nop; nop; nop; nop; nop; nop;"
         "nop; nop; nop; nop; nop; nop; nop; nop;"
 #endif
 #if CH32_F_CPU >= 120000000
         "nop; nop; nop; nop; nop; nop; nop; nop;"
         "nop; nop; nop; nop; nop; nop; nop; nop;"
         "nop; nop; nop; nop; nop; nop; nop; nop;"
 #endif
 #if CH32_F_CPU >= 144000000
         "nop; nop; nop; nop; nop; nop; nop; nop;"
         "nop; nop; nop; nop; nop; nop; nop; nop;"
 #endif
         );
 
       // Low 450ns
       *clr = ch_pin;
       __asm volatile ("nop; nop; nop; nop; nop; nop; nop; nop;"
         "nop; nop; nop; nop; nop; nop; nop; nop;"
         "nop;"
 #if CH32_F_CPU >= 72000000
         "nop; nop; nop; nop; nop; nop; nop; nop; nop;"
 #endif
 #if CH32_F_CPU >= 96000000
         "nop; nop; nop; nop; nop; nop;"
 #endif
 #if CH32_F_CPU >= 120000000
         "nop; nop; nop; nop; nop;"
         "nop; nop; nop; nop; nop; nop; nop; nop;"
 #endif
 #if CH32_F_CPU >= 144000000
         "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;"
 #endif
         );
     } else {   // ZERO
       // High 400ns
       *set = ch_pin;
       __asm volatile ("nop; nop; nop; nop; nop; nop; nop; nop;"
         "nop; nop; nop; nop; nop; nop; nop; nop;"
         "nop;"
 #if CH32_F_CPU >= 72000000
         "nop; nop; nop; nop; nop; nop; nop;"
 #endif
 #if CH32_F_CPU >= 96000000
         "nop; nop; nop; nop; nop; nop; nop; nop;"
 #endif
 #if CH32_F_CPU >= 120000000
         "nop; nop; nop; "
         "nop; nop; nop; nop; nop; nop; nop; nop;"
 #endif
 #if CH32_F_CPU >= 144000000
         "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;"
 #endif
         );
 
       // Low 850ns
       *clr = ch_pin;
       __asm volatile ("nop; nop; nop; nop; nop; nop; nop; nop;"
         "nop; nop; nop; nop; nop; nop; nop; nop;"
         "nop; nop; nop; nop; nop; nop; nop; nop;"
         "nop; nop; nop; nop; nop; nop; nop; nop;"
         "nop; nop; nop; nop; nop;"
 #if CH32_F_CPU >= 72000000
         "nop; nop; nop;"
         "nop; nop; nop; nop; nop; nop; nop; nop;"
 #endif
 #if CH32_F_CPU >= 96000000
         "nop; nop; nop; nop; nop; nop; nop; nop;"
         "nop; nop; nop; nop; nop; nop; nop; nop;"
         "nop; nop; nop; nop; nop; nop;"
 #endif
 #if CH32_F_CPU >= 120000000
         "nop; nop; nop; nop; nop; nop; nop; nop;"
         "nop; nop; nop; nop; nop; nop; nop; nop;"
         "nop; nop; nop; nop; nop; nop;"
 #endif
 #if CH32_F_CPU >= 144000000
         "nop; nop; nop; nop;"
         "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;"
 #endif
         );
     }
 
     if (bitMask >>= 1) {
       // Move on to the next pixel
       asm("nop;");
     }
     else {
       if (ptr >= end) {
         break;
       }
       p = *ptr++;
       bitMask = 0x80;
     }
   }
 
   // NVIC_EnableIRQ(SysTicK_IRQn);
 }
 #endif
 
 #if defined(ESP8266)
 // ESP8266 show() is external to enforce ICACHE_RAM_ATTR execution
 extern "C" IRAM_ATTR void espShow(uint16_t pin, uint8_t *pixels,
                                   uint32_t numBytes, uint8_t type);
 #elif defined(ESP32)
 extern "C" void espShow(uint16_t pin, uint8_t *pixels, uint32_t numBytes,
                         uint8_t type);
 
 #endif // ESP8266
 
 #if defined(K210)
 #define KENDRYTE_K210 1
 #endif
 
 #if defined(KENDRYTE_K210)
 extern "C" void k210Show(uint8_t pin, uint8_t *pixels, uint32_t numBytes,
                          boolean is800KHz);
 #endif // KENDRYTE_K210
 /*!
   @brief   Transmit pixel data in RAM to NeoPixels.
   @note    On most architectures, interrupts are temporarily disabled in
            order to achieve the correct NeoPixel signal timing. This means
            that the Arduino millis() and micros() functions, which require
            interrupts, will lose small intervals of time whenever this
            function is called (about 30 microseconds per RGB pixel, 40 for
            RGBW pixels). There's no easy fix for this, but a few
            specialized alternative or companion libraries exist that use
            very device-specific peripherals to work around it.
 */
 void Adafruit_NeoPixel::show(void) {
 
   if (!pixels)
     return;
 
   // Data latch = 300+ microsecond pause in the output stream. Rather than
   // put a delay at the end of the function, the ending time is noted and
   // the function will simply hold off (if needed) on issuing the
   // subsequent round of data until the latch time has elapsed. This
   // allows the mainline code to start generating the next frame of data
   // rather than stalling for the latch.
   while (!canShow())
     ;
     // endTime is a private member (rather than global var) so that multiple
     // instances on different pins can be quickly issued in succession (each
     // instance doesn't delay the next).
 
     // In order to make this code runtime-configurable to work with any pin,
     // SBI/CBI instructions are eschewed in favor of full PORT writes via the
     // OUT or ST instructions. It relies on two facts: that peripheral
     // functions (such as PWM) take precedence on output pins, so our PORT-
     // wide writes won't interfere, and that interrupts are globally disabled
     // while data is being issued to the LEDs, so no other code will be
     // accessing the PORT. The code takes an initial 'snapshot' of the PORT
     // state, computes 'pin high' and 'pin low' values, and writes these back
     // to the PORT register as needed.
 
   // NRF52 may use PWM + DMA (if available), may not need to disable interrupt
   // ESP32 may not disable interrupts because espShow() uses RMT which tries to acquire locks
 #if !(defined(NRF52) || defined(NRF52_SERIES) || defined(ESP32))
   noInterrupts(); // Need 100% focus on instruction timing
 #endif
 
 
 #if defined(__arm__)
 
     // ARM MCUs -- Teensy 3.0, 3.1, LC, Arduino Due, RP2040 -------------------
 
 #if defined(ARDUINO_ARCH_RENESAS) || defined(ARDUINO_ARCH_RENESAS_UNO) || defined(ARDUINO_ARCH_RENESAS_PORTENTA)
 
 // Definition for a single channel clockless controller for RA4M1 (Cortex M4)
 // See clockless.h for detailed info on how the template parameters are used.
 #define ARM_DEMCR               (*(volatile uint32_t *)0xE000EDFC) // Debug Exception and Monitor Control
 #define ARM_DEMCR_TRCENA                (1 << 24)        // Enable debugging & monitoring blocks
 #define ARM_DWT_CTRL            (*(volatile uint32_t *)0xE0001000) // DWT control register
 #define ARM_DWT_CTRL_CYCCNTENA          (1 << 0)                // Enable cycle count
 #define ARM_DWT_CYCCNT          (*(volatile uint32_t *)0xE0001004) // Cycle count register
 
 #define F_CPU 48000000
 #define CYCLES_800_T0H (F_CPU / 4000000)
 #define CYCLES_800_T1H (F_CPU / 1250000)
 #define CYCLES_800 (F_CPU / 800000)
 #define CYCLES_400_T0H (F_CPU / 2000000)
 #define CYCLES_400_T1H (F_CPU / 833333)
 #define CYCLES_400 (F_CPU / 400000)
 
   uint8_t *p = pixels, *end = p + numBytes, pix, mask;
 
   bsp_io_port_pin_t io_pin = g_pin_cfg[pin].pin;
   #define PIN_IO_PORT_ADDR(pn)      (R_PORT0 + ((uint32_t) (R_PORT1 - R_PORT0) * ((pn) >> 8u)))
 
   volatile uint16_t *set = &(PIN_IO_PORT_ADDR(io_pin)->POSR);
   volatile uint16_t *clr = &(PIN_IO_PORT_ADDR(io_pin)->PORR);
   uint16_t msk = (1U << (io_pin & 0xFF));
 
   uint32_t cyc;
 
   ARM_DEMCR |= ARM_DEMCR_TRCENA;
   ARM_DWT_CTRL |= ARM_DWT_CTRL_CYCCNTENA;
 
 #if defined(NEO_KHZ400) // 800 KHz check needed only if 400 KHz support enabled
   if (is800KHz) {
 #endif
     cyc = ARM_DWT_CYCCNT + CYCLES_800;
     while (p < end) {
       pix = *p++;
       for (mask = 0x80; mask; mask >>= 1) {
         while (ARM_DWT_CYCCNT - cyc < CYCLES_800)
           ;
         cyc = ARM_DWT_CYCCNT;
         *set = msk;
         if (pix & mask) {
           while (ARM_DWT_CYCCNT - cyc < CYCLES_800_T1H)
             ;
         } else {
           while (ARM_DWT_CYCCNT - cyc < CYCLES_800_T0H)
             ;
         }
         *clr = msk;
       }
     }
     while (ARM_DWT_CYCCNT - cyc < CYCLES_800)
       ;
 #if defined(NEO_KHZ400)
   } else { // 400 kHz bitstream
     cyc = ARM_DWT_CYCCNT + CYCLES_400;
     while (p < end) {
       pix = *p++;
       for (mask = 0x80; mask; mask >>= 1) {
         while (ARM_DWT_CYCCNT - cyc < CYCLES_400)
           ;
         cyc = ARM_DWT_CYCCNT;
         *set = msk;
         if (pix & mask) {
           while (ARM_DWT_CYCCNT - cyc < CYCLES_400_T1H)
             ;
         } else {
           while (ARM_DWT_CYCCNT - cyc < CYCLES_400_T0H)
             ;
         }
         *clr = msk;
       }
     }
     while (ARM_DWT_CYCCNT - cyc < CYCLES_400)
       ;
   }
 #endif // NEO_KHZ400
 
 #endif // ARM
 
   // END ARM ----------------------------------------------------------------
 
 
 #else
 #error Architecture not supported
 #endif
 
   // END ARCHITECTURE SELECT ------------------------------------------------
 
 #if !(defined(NRF52) || defined(NRF52_SERIES) || defined(ESP32))
   interrupts();
 #endif
 
   endTime = micros(); // Save EOD time for latch on next call
 }
 
 /*!
   @brief   Set/change the NeoPixel output pin number. Previous pin,
            if any, is set to INPUT and the new pin is set to OUTPUT.
   @param   p  Arduino pin number (-1 = no pin).
 */
 void Adafruit_NeoPixel::setPin(int16_t p) {
   if (begun && (pin >= 0))
     pinMode(pin, INPUT); // Disable existing out pin
   pin = p;
   if (begun) {
     pinMode(p, OUTPUT);
     digitalWrite(p, LOW);
   }
 #if defined(__AVR__)
   port = portOutputRegister(digitalPinToPort(p));
   pinMask = digitalPinToBitMask(p);
 #endif
 #if defined(ARDUINO_ARCH_STM32) || defined(ARDUINO_ARCH_ARDUINO_CORE_STM32)
   gpioPort = digitalPinToPort(p);
   gpioPin = STM_LL_GPIO_PIN(digitalPinToPinName(p));
 #elif defined(ARDUINO_ARCH_CH32)
   PinName const pin_name = digitalPinToPinName(pin);
   gpioPort = get_GPIO_Port(CH_PORT(pin_name));
   gpioPin = CH_GPIO_PIN(pin_name);
   #if defined (CH32V20x_D6)
   if (gpioPort == GPIOC && ((*(volatile uint32_t*)0x40022030) & 0x0F000000) == 0) {
     gpioPin = gpioPin >> 13;
   }
   #endif
 #endif
 }
 
 /*!
   @brief   Set a pixel's color using separate red, green and blue
            components. If using RGBW pixels, white will be set to 0.
   @param   n  Pixel index, starting from 0.
   @param   r  Red brightness, 0 = minimum (off), 255 = maximum.
   @param   g  Green brightness, 0 = minimum (off), 255 = maximum.
   @param   b  Blue brightness, 0 = minimum (off), 255 = maximum.
 */
 void Adafruit_NeoPixel::setPixelColor(uint16_t n, uint8_t r, uint8_t g,
                                       uint8_t b) {
 
   if (n < numLEDs) {
     if (brightness) { // See notes in setBrightness()
       r = (r * brightness) >> 8;
       g = (g * brightness) >> 8;
       b = (b * brightness) >> 8;
     }
     uint8_t *p;
     if (wOffset == rOffset) { // Is an RGB-type strip
       p = &pixels[n * 3];     // 3 bytes per pixel
     } else {                  // Is a WRGB-type strip
       p = &pixels[n * 4];     // 4 bytes per pixel
       p[wOffset] = 0;         // But only R,G,B passed -- set W to 0
     }
     p[rOffset] = r; // R,G,B always stored
     p[gOffset] = g;
     p[bOffset] = b;
   }
 }
 
 /*!
   @brief   Set a pixel's color using separate red, green, blue and white
            components (for RGBW NeoPixels only).
   @param   n  Pixel index, starting from 0.
   @param   r  Red brightness, 0 = minimum (off), 255 = maximum.
   @param   g  Green brightness, 0 = minimum (off), 255 = maximum.
   @param   b  Blue brightness, 0 = minimum (off), 255 = maximum.
   @param   w  White brightness, 0 = minimum (off), 255 = maximum, ignored
               if using RGB pixels.
 */
 void Adafruit_NeoPixel::setPixelColor(uint16_t n, uint8_t r, uint8_t g,
                                       uint8_t b, uint8_t w) {
 
   if (n < numLEDs) {
     if (brightness) { // See notes in setBrightness()
       r = (r * brightness) >> 8;
       g = (g * brightness) >> 8;
       b = (b * brightness) >> 8;
       w = (w * brightness) >> 8;
     }
     uint8_t *p;
     if (wOffset == rOffset) { // Is an RGB-type strip
       p = &pixels[n * 3];     // 3 bytes per pixel (ignore W)
     } else {                  // Is a WRGB-type strip
       p = &pixels[n * 4];     // 4 bytes per pixel
       p[wOffset] = w;         // Store W
     }
     p[rOffset] = r; // Store R,G,B
     p[gOffset] = g;
     p[bOffset] = b;
   }
 }
 
 /*!
   @brief   Set a pixel's color using a 32-bit 'packed' RGB or RGBW value.
   @param   n  Pixel index, starting from 0.
   @param   c  32-bit color value. Most significant byte is white (for RGBW
               pixels) or ignored (for RGB pixels), next is red, then green,
               and least significant byte is blue.
 */
 void Adafruit_NeoPixel::setPixelColor(uint16_t n, uint32_t c) {
   if (n < numLEDs) {
     uint8_t *p, r = (uint8_t)(c >> 16), g = (uint8_t)(c >> 8), b = (uint8_t)c;
     if (brightness) { // See notes in setBrightness()
       r = (r * brightness) >> 8;
       g = (g * brightness) >> 8;
       b = (b * brightness) >> 8;
     }
     if (wOffset == rOffset) {
       p = &pixels[n * 3];
     } else {
       p = &pixels[n * 4];
       uint8_t w = (uint8_t)(c >> 24);
       p[wOffset] = brightness ? ((w * brightness) >> 8) : w;
     }
     p[rOffset] = r;
     p[gOffset] = g;
     p[bOffset] = b;
   }
 }
 
 /*!
   @brief   Fill all or part of the NeoPixel strip with a color.
   @param   c      32-bit color value. Most significant byte is white (for
                   RGBW pixels) or ignored (for RGB pixels), next is red,
                   then green, and least significant byte is blue. If all
                   arguments are unspecified, this will be 0 (off).
   @param   first  Index of first pixel to fill, starting from 0. Must be
                   in-bounds, no clipping is performed. 0 if unspecified.
   @param   count  Number of pixels to fill, as a positive value. Passing
                   0 or leaving unspecified will fill to end of strip.
 */
 void Adafruit_NeoPixel::fill(uint32_t c, uint16_t first, uint16_t count) {
   uint16_t i, end;
 
   if (first >= numLEDs) {
     return; // If first LED is past end of strip, nothing to do
   }
 
   // Calculate the index ONE AFTER the last pixel to fill
   if (count == 0) {
     // Fill to end of strip
     end = numLEDs;
   } else {
     // Ensure that the loop won't go past the last pixel
     end = first + count;
     if (end > numLEDs)
       end = numLEDs;
   }
 
   for (i = first; i < end; i++) {
     this->setPixelColor(i, c);
   }
 }
 
 /*!
   @brief   Convert hue, saturation and value into a packed 32-bit RGB color
            that can be passed to setPixelColor() or other RGB-compatible
            functions.
   @param   hue  An unsigned 16-bit value, 0 to 65535, representing one full
                 loop of the color wheel, which allows 16-bit hues to "roll
                 over" while still doing the expected thing (and allowing
                 more precision than the wheel() function that was common to
                 prior NeoPixel examples).
   @param   sat  Saturation, 8-bit value, 0 (min or pure grayscale) to 255
                 (max or pure hue). Default of 255 if unspecified.
   @param   val  Value (brightness), 8-bit value, 0 (min / black / off) to
                 255 (max or full brightness). Default of 255 if unspecified.
   @return  Packed 32-bit RGB with the most significant byte set to 0 -- the
            white element of WRGB pixels is NOT utilized. Result is linearly
            but not perceptually correct, so you may want to pass the result
            through the gamma32() function (or your own gamma-correction
            operation) else colors may appear washed out. This is not done
            automatically by this function because coders may desire a more
            refined gamma-correction function than the simplified
            one-size-fits-all operation of gamma32(). Diffusing the LEDs also
            really seems to help when using low-saturation colors.
 */
 uint32_t Adafruit_NeoPixel::ColorHSV(uint16_t hue, uint8_t sat, uint8_t val) {
 
   uint8_t r, g, b;
 
   // Remap 0-65535 to 0-1529. Pure red is CENTERED on the 64K rollover;
   // 0 is not the start of pure red, but the midpoint...a few values above
   // zero and a few below 65536 all yield pure red (similarly, 32768 is the
   // midpoint, not start, of pure cyan). The 8-bit RGB hexcone (256 values
   // each for red, green, blue) really only allows for 1530 distinct hues
   // (not 1536, more on that below), but the full unsigned 16-bit type was
   // chosen for hue so that one's code can easily handle a contiguous color
   // wheel by allowing hue to roll over in either direction.
   hue = (hue * 1530L + 32768) / 65536;
   // Because red is centered on the rollover point (the +32768 above,
   // essentially a fixed-point +0.5), the above actually yields 0 to 1530,
   // where 0 and 1530 would yield the same thing. Rather than apply a
   // costly modulo operator, 1530 is handled as a special case below.
 
   // So you'd think that the color "hexcone" (the thing that ramps from
   // pure red, to pure yellow, to pure green and so forth back to red,
   // yielding six slices), and with each color component having 256
   // possible values (0-255), might have 1536 possible items (6*256),
   // but in reality there's 1530. This is because the last element in
   // each 256-element slice is equal to the first element of the next
   // slice, and keeping those in there this would create small
   // discontinuities in the color wheel. So the last element of each
   // slice is dropped...we regard only elements 0-254, with item 255
   // being picked up as element 0 of the next slice. Like this:
   // Red to not-quite-pure-yellow is:        255,   0, 0 to 255, 254,   0
   // Pure yellow to not-quite-pure-green is: 255, 255, 0 to   1, 255,   0
   // Pure green to not-quite-pure-cyan is:     0, 255, 0 to   0, 255, 254
   // and so forth. Hence, 1530 distinct hues (0 to 1529), and hence why
   // the constants below are not the multiples of 256 you might expect.
 
   // Convert hue to R,G,B (nested ifs faster than divide+mod+switch):
   if (hue < 510) { // Red to Green-1
     b = 0;
     if (hue < 255) { //   Red to Yellow-1
       r = 255;
       g = hue;       //     g = 0 to 254
     } else {         //   Yellow to Green-1
       r = 510 - hue; //     r = 255 to 1
       g = 255;
     }
   } else if (hue < 1020) { // Green to Blue-1
     r = 0;
     if (hue < 765) { //   Green to Cyan-1
       g = 255;
       b = hue - 510;  //     b = 0 to 254
     } else {          //   Cyan to Blue-1
       g = 1020 - hue; //     g = 255 to 1
       b = 255;
     }
   } else if (hue < 1530) { // Blue to Red-1
     g = 0;
     if (hue < 1275) { //   Blue to Magenta-1
       r = hue - 1020; //     r = 0 to 254
       b = 255;
     } else { //   Magenta to Red-1
       r = 255;
       b = 1530 - hue; //     b = 255 to 1
     }
   } else { // Last 0.5 Red (quicker than % operator)
     r = 255;
     g = b = 0;
   }
 
   // Apply saturation and value to R,G,B, pack into 32-bit result:
   uint32_t v1 = 1 + val;  // 1 to 256; allows >>8 instead of /255
   uint16_t s1 = 1 + sat;  // 1 to 256; same reason
   uint8_t s2 = 255 - sat; // 255 to 0
   return ((((((r * s1) >> 8) + s2) * v1) & 0xff00) << 8) |
          (((((g * s1) >> 8) + s2) * v1) & 0xff00) |
          (((((b * s1) >> 8) + s2) * v1) >> 8);
 }
 
 /*!
   @brief   Query the color of a previously-set pixel.
   @param   n  Index of pixel to read (0 = first).
   @return  'Packed' 32-bit RGB or WRGB value. Most significant byte is white
            (for RGBW pixels) or 0 (for RGB pixels), next is red, then green,
            and least significant byte is blue.
   @note    If the strip brightness has been changed from the default value
            of 255, the color read from a pixel may not exactly match what
            was previously written with one of the setPixelColor() functions.
            This gets more pronounced at lower brightness levels.
 */
 uint32_t Adafruit_NeoPixel::getPixelColor(uint16_t n) const {
   if (n >= numLEDs)
     return 0; // Out of bounds, return no color.
 
   uint8_t *p;
 
   if (wOffset == rOffset) { // Is RGB-type device
     p = &pixels[n * 3];
     if (brightness) {
       // Stored color was decimated by setBrightness(). Returned value
       // attempts to scale back to an approximation of the original 24-bit
       // value used when setting the pixel color, but there will always be
       // some error -- those bits are simply gone. Issue is most
       // pronounced at low brightness levels.
       return (((uint32_t)(p[rOffset] << 8) / brightness) << 16) |
              (((uint32_t)(p[gOffset] << 8) / brightness) << 8) |
              ((uint32_t)(p[bOffset] << 8) / brightness);
     } else {
       // No brightness adjustment has been made -- return 'raw' color
       return ((uint32_t)p[rOffset] << 16) | ((uint32_t)p[gOffset] << 8) |
              (uint32_t)p[bOffset];
     }
   } else { // Is RGBW-type device
     p = &pixels[n * 4];
     if (brightness) { // Return scaled color
       return (((uint32_t)(p[wOffset] << 8) / brightness) << 24) |
              (((uint32_t)(p[rOffset] << 8) / brightness) << 16) |
              (((uint32_t)(p[gOffset] << 8) / brightness) << 8) |
              ((uint32_t)(p[bOffset] << 8) / brightness);
     } else { // Return raw color
       return ((uint32_t)p[wOffset] << 24) | ((uint32_t)p[rOffset] << 16) |
              ((uint32_t)p[gOffset] << 8) | (uint32_t)p[bOffset];
     }
   }
 }
 
 /*!
   @brief   Adjust output brightness. Does not immediately affect what's
            currently displayed on the LEDs. The next call to show() will
            refresh the LEDs at this level.
   @param   b  Brightness setting, 0=minimum (off), 255=brightest.
   @note    This was intended for one-time use in one's setup() function,
            not as an animation effect in itself. Because of the way this
            library "pre-multiplies" LED colors in RAM, changing the
            brightness is often a "lossy" operation -- what you write to
            pixels isn't necessary the same as what you'll read back.
            Repeated brightness changes using this function exacerbate the
            problem. Smart programs therefore treat the strip as a
            write-only resource, maintaining their own state to render each
            frame of an animation, not relying on read-modify-write.
 */
 void Adafruit_NeoPixel::setBrightness(uint8_t b) {
   // Stored brightness value is different than what's passed.
   // This simplifies the actual scaling math later, allowing a fast
   // 8x8-bit multiply and taking the MSB. 'brightness' is a uint8_t,
   // adding 1 here may (intentionally) roll over...so 0 = max brightness
   // (color values are interpreted literally; no scaling), 1 = min
   // brightness (off), 255 = just below max brightness.
   uint8_t newBrightness = b + 1;
   if (newBrightness != brightness) { // Compare against prior value
     // Brightness has changed -- re-scale existing data in RAM,
     // This process is potentially "lossy," especially when increasing
     // brightness. The tight timing in the WS2811/WS2812 code means there
     // aren't enough free cycles to perform this scaling on the fly as data
     // is issued. So we make a pass through the existing color data in RAM
     // and scale it (subsequent graphics commands also work at this
     // brightness level). If there's a significant step up in brightness,
     // the limited number of steps (quantization) in the old data will be
     // quite visible in the re-scaled version. For a non-destructive
     // change, you'll need to re-render the full strip data. C'est la vie.
     uint8_t c, *ptr = pixels,
                oldBrightness = brightness - 1; // De-wrap old brightness value
     uint16_t scale;
     if (oldBrightness == 0)
       scale = 0; // Avoid /0
     else if (b == 255)
       scale = 65535 / oldBrightness;
     else
       scale = (((uint16_t)newBrightness << 8) - 1) / oldBrightness;
     for (uint16_t i = 0; i < numBytes; i++) {
       c = *ptr;
       *ptr++ = (c * scale) >> 8;
     }
     brightness = newBrightness;
   }
 }
 
 /*!
   @brief   Retrieve the last-set brightness value for the strip.
   @return  Brightness value: 0 = minimum (off), 255 = maximum.
 */
 uint8_t Adafruit_NeoPixel::getBrightness(void) const { return brightness - 1; }
 
 /*!
   @brief   Fill the whole NeoPixel strip with 0 / black / off.
 */
 void Adafruit_NeoPixel::clear(void) { memset(pixels, 0, numBytes); }
 
 // A 32-bit variant of gamma8() that applies the same function
 // to all components of a packed RGB or WRGB value.
 uint32_t Adafruit_NeoPixel::gamma32(uint32_t x) {
   uint8_t *y = (uint8_t *)&x;
   // All four bytes of a 32-bit value are filtered even if RGB (not WRGB),
   // to avoid a bunch of shifting and masking that would be necessary for
   // properly handling different endianisms (and each byte is a fairly
   // trivial operation, so it might not even be wasting cycles vs a check
   // and branch for the RGB case). In theory this might cause trouble *if*
   // someone's storing information in the unused most significant byte
   // of an RGB value, but this seems exceedingly rare and if it's
   // encountered in reality they can mask values going in or coming out.
   for (uint8_t i = 0; i < 4; i++)
     y[i] = gamma8(y[i]);
   return x; // Packed 32-bit return
 }
 
 /*!
   @brief   Fill NeoPixel strip with one or more cycles of hues.
            Everyone loves the rainbow swirl so much, now it's canon!
   @param   first_hue   Hue of first pixel, 0-65535, representing one full
                        cycle of the color wheel. Each subsequent pixel will
                        be offset to complete one or more cycles over the
                        length of the strip.
   @param   reps        Number of cycles of the color wheel over the length
                        of the strip. Default is 1. Negative values can be
                        used to reverse the hue order.
   @param   saturation  Saturation (optional), 0-255 = gray to pure hue,
                        default = 255.
   @param   brightness  Brightness/value (optional), 0-255 = off to max,
                        default = 255. This is distinct and in combination
                        with any configured global strip brightness.
   @param   gammify     If true (default), apply gamma correction to colors
                        for better appearance.
 */
 void Adafruit_NeoPixel::rainbow(uint16_t first_hue, int8_t reps,
   uint8_t saturation, uint8_t brightness, bool gammify) {
   for (uint16_t i=0; i<numLEDs; i++) {
     uint16_t hue = first_hue + (i * reps * 65536) / numLEDs;
     uint32_t color = ColorHSV(hue, saturation, brightness);
     if (gammify) color = gamma32(color);
     setPixelColor(i, color);
   }
 }
 
 /*!
   @brief  Convert pixel color order from string (e.g. "BGR") to NeoPixel
           color order constant (e.g. NEO_BGR). This may be helpful for code
           that initializes from text configuration rather than compile-time
           constants.
   @param   v  Input string. Should be reasonably sanitized (a 3- or 4-
               character NUL-terminated string) or undefined behavior may
               result (output is still a valid NeoPixel order constant, but
               might not present as expected). Garbage in, garbage out.
   @return  One of the NeoPixel color order constants (e.g. NEO_BGR).
            NEO_KHZ400 or NEO_KHZ800 bits are not included, nor needed (all
            NeoPixels actually support 800 KHz it's been found, and this is
            the default state if no KHZ bits set).
   @note    This function is declared static in the class so it can be called
            without a NeoPixel object (since it's not likely been declared
            in the code yet). Use Adafruit_NeoPixel::str2order().
 */
 neoPixelType Adafruit_NeoPixel::str2order(const char *v) {
   int8_t r = 0, g = 0, b = 0, w = -1;
   if (v) {
     char c;
     for (uint8_t i=0; ((c = tolower(v[i]))); i++) {
       if (c == 'r') r = i;
       else if (c == 'g') g = i;
       else if (c == 'b') b = i;
       else if (c == 'w') w = i;
     }
     r &= 3;
   }
   if (w < 0) w = r; // If 'w' not specified, duplicate r bits
   return (w << 6) | (r << 4) | ((g & 3) << 2) | (b & 3);
 }
 