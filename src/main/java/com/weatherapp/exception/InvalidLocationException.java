package com.weatherapp.exception;

public class InvalidLocationException extends Exception {
    public InvalidLocationException(String message) {
        super(message);
    }
}
