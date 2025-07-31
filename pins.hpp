#pragma once

#define LED_RED 86
#define LED_GREEN 87
#define LED_BLUE 88

namespace pins
{
    /**
     * @brief Set the built-in RGB LED to red.
     */
    void red()
    {
        digitalWrite(LED_RED, LOW);
        digitalWrite(LED_GREEN, HIGH);
        digitalWrite(LED_BLUE, HIGH);
    }

    /**
     * @brief Set the built-in RGB LED to green.
     */
    void green()
    {
        digitalWrite(LED_RED, HIGH);
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_BLUE, HIGH);
    }

    /**
     * @brief Set the built-in RGB LED to blue.
     */
    void blue()
    {
        digitalWrite(LED_RED, HIGH);
        digitalWrite(LED_GREEN, HIGH);
        digitalWrite(LED_BLUE, LOW);
    }

    /**
     * @brief Set the built-in RGB LED to cyan.
     */
    void cyan()
    {
        digitalWrite(LED_RED, HIGH);
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_BLUE, LOW);
    }

    /**
     * @brief Set the built-in RGB LED to magenta.
     */
    void magenta()
    {
        digitalWrite(LED_RED, LOW);
        digitalWrite(LED_GREEN, HIGH);
        digitalWrite(LED_BLUE, LOW);
    }

    /**
     * @brief Set the built-in RGB LED to yellow.
     */
    void yellow()
    {
        digitalWrite(LED_RED, LOW);
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_BLUE, HIGH);
    }

    /**
     * @brief Set the built-in RGB LED to white.
     */
    void white()
    {
        digitalWrite(LED_RED, LOW);
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_BLUE, LOW);
    }

    /**
     * @brief Turn off the built-in RGB LED.
     */
    void off()
    {
        digitalWrite(LED_RED, HIGH);
        digitalWrite(LED_GREEN, HIGH);
        digitalWrite(LED_BLUE, HIGH);
    }

    /**
     * @brief Turn off the built-in RGB LED.
     */
    inline void black()
    {
        off();
    }

    /**
     * @brief Initialize the pins for the built-in RGB LED.
     */
    void init()
    {
        pinMode(LED_RED, OUTPUT);
        pinMode(LED_GREEN, OUTPUT);
        pinMode(LED_BLUE, OUTPUT);
        off();
    }
}