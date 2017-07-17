package com.mycompany.server;

import java.io.IOException;
import java.io.PrintWriter;
import java.sql.ResultSet;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

public class payment extends HttpServlet 
{
    public String password = "chooseyourkey";

    MySQL db = new MySQL();

    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException 
    {   
        String data = request.getParameter("Id");
        data = Crypto.Decrypt(data, password);
        
        db.InitializeStatement();
            
        boolean rs = db.InsertQuery("UPDATE host SET Payment = true WHERE IdHost='"+ data +"'");
        
        if (rs==true)
        {
            String IdKey = "true";
            String rsCrypto = Crypto.Encrypt(IdKey, password);
                
            PrintWriter out = response.getWriter();
            out.println(rsCrypto);
            out.flush();
        }
        else
        {
            String IdKeyF = "false";
            String rsCryptoF = Crypto.Encrypt(IdKeyF, password);
                
            PrintWriter out = response.getWriter();
            out.println(rsCryptoF);
            out.flush();
        }
        
        db.CloseStatement();
    }

    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException 
    {
        doGet(request, response);
    }
    
    @Override
    public String getServletInfo() 
    {
        return "Short description";
    }
}