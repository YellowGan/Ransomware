package com.mycompany.server;

import java.util.Random;

public class RandomGenerator
{    
    public String generateSessionKey(int lenght)
    {
        String alphabet = new String("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");        
        int n = alphabet.length();

        String result = new String(); 
        Random r = new Random();

        for (int i=0; i<lenght; i++)
            result = result + alphabet.charAt(r.nextInt(n));

        return result;
    }    
}