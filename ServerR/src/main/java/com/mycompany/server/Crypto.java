package com.mycompany.server;

import javax.xml.bind.DatatypeConverter;

public class Crypto
{   
    public static String Encrypt(String input, String key)
    {
        input = DatatypeConverter.printBase64Binary(input.getBytes());

        String tmp = key;

        while (key.length() < input.length())
            key += tmp;

        StringBuilder output = new StringBuilder("");

        for (int i = 0; i < input.length(); ++i)
        {
            output.append((char)(input.charAt(i) ^ key.charAt(i)));
        }

        String out = DatatypeConverter.printBase64Binary(output.toString().getBytes());
        return out;
    }

    public static String Decrypt(String input, String key)
    {
        byte[] decoded = DatatypeConverter.parseBase64Binary(input);
        input = new String(decoded);

        String tmp = key;

        while (key.length() < input.length())
            key += tmp;

        StringBuilder output = new StringBuilder("");

        for (int i = 0; i < input.length(); ++i)
        {
            output.append((char)(input.charAt(i) ^ key.charAt(i)));
        }

        decoded = DatatypeConverter.parseBase64Binary(output.toString());
        String out = new String(decoded);
        return out;
    }   
}