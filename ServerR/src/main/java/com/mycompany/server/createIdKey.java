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

public class createIdKey extends HttpServlet
{
    public String password = "chooseyourkey";

    MySQL db = new MySQL();
    RandomGenerator id = new RandomGenerator();
    
    public void checkId (String idFrom)
    {
        ResultSet rs = db.SelectionQuery("SELECT IdHost FROM host");        
        String result = "";

        try 
        {
            while (rs.next())
            {
               result = rs.getString("IdHost");
               System.out.println("result " + result);
               
               if (idFrom.equals(result))
               {
                   idFrom = id.generateSessionKey(50);
                   checkId(idFrom);
               }
            }
        } 
        catch (SQLException ex) 
        {
            Logger.getLogger(checkPayment.class.getName()).log(Level.SEVERE, null, ex);
        }
            
            
    }
    
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException 
    {        
        String data = request.getParameter("pass");
        data = Crypto.Decrypt(data, password);
        
        db.InitializeStatement();
        
        String idHost = id.generateSessionKey(50);
        checkId(idHost);
                    
        boolean rs2 = db.InsertQuery("INSERT INTO host (IdHost,KeyHost,Payment) VALUES ('"+idHost+"','"+data+"',false)");                
        String IdKey = "" + idHost + "";
        String rsCrypto = Crypto.Encrypt(IdKey, password);                
        
        PrintWriter out = response.getWriter();
        out.println(rsCrypto);
        out.flush();
        
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