/*
 * The jdbc class
 * Used to connect the SQLserver with the application server
 * The url for the SQLserver and credentials should be given below
 */
package Server;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.Statement;
import javax.swing.JOptionPane;


public class jdbc {
    
//    Set connection driver and database url
    String driver="com.mysql.jdbc.Driver";
    String url="jdbc:mysql://localhost:3306/tst"; // SQLserver URL and database name
 
//    Set the connection with SQL Server
    Connection getConnection() throws  Exception{
        Class.forName(driver);
        Connection con= DriverManager.getConnection(url, "root", "");   // Server credentials
        return con;
    }
    
//    Send data to databse
    void putdata(String sql){
        try {
            Statement state=getConnection().createStatement();
            state.executeUpdate(sql);
        } catch (Exception e) {
            JOptionPane.showMessageDialog(null, e);
        }
    }
//    Get data from database
    ResultSet getdata(String sql) throws  Exception{
        Statement state=getConnection().createStatement();
        ResultSet rset=state.executeQuery(sql);
        return rset;
    }
    
}
