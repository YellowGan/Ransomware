package com.mycompany.server;

import java.io.IOException;
import java.io.PrintWriter;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

public class checkPayment extends HttpServlet 
{   
    String password = "chooseyourkey";

    MySQL db = new MySQL();
    
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
    {
        String data = request.getParameter("Id");
        data = Crypto.Decrypt(data, password);
        
        db.Initialize();           
        ResultSet rsPay = db.SelectionQuery("SELECT Payment FROM host WHERE IdHost='"+data+"'");
            
        String result = "";

        try 
        {
            while (rsPay.next())
            {
               result = rsPay.getString("Payment");
               System.out.println("result " + result);
            }
        } 
        catch (SQLException ex) 
        {
            Logger.getLogger(checkPayment.class.getName()).log(Level.SEVERE, null, ex);
        }
            
            
        if (result.equals("1"))
        {
            String IdKey = "true";            
            System.out.println(IdKey);
            String rsCrypto = Crypto.Encrypt(IdKey, password);
                
            PrintWriter out = response.getWriter();
            out.println(rsCrypto);
            out.flush();
        }
        else
        {
            String IdKeyF = "fals";
            String rsCryptoF = Crypto.Encrypt(IdKeyF, password);
            
            PrintWriter out = response.getWriter();
            out.println(rsCryptoF);
            out.flush();
        }
            
        db.Close();
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