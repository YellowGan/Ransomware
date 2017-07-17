package com.mycompany.server;

import java.io.IOException;
import java.io.PrintWriter;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

public class delete extends HttpServlet
{
    public String password = "chooseyourkey";

    MySQL db = new MySQL();

    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException 
    {    
        String data = request.getParameter("Id");
        data = Crypto.Decrypt(data, password);        
        
        db.InitializeStatement();
            
        boolean rsDelete = db.RemoveQuery("DELETE FROM host WHERE IdHost='"+data+"'");

        if(rsDelete == true)
        {
            String rs = "true";            
            String rsCrypto = Crypto.Encrypt(rs, password);

            PrintWriter out = response.getWriter();
            out.println(rsCrypto);
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