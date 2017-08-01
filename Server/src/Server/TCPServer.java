/*
 * The server class
 * This server and the device should run on the same subnet
 * 
 */
package Server;

import java.io.*;
import java.net.*;
import java.sql.ResultSet;
import java.util.ArrayList;

public class TCPServer {
    
//    Define server socket
    static ServerSocket welcomeSocket;
    
//    Define student details
    static String studID;
    static String studName;
    static String subCode;
    static ArrayList<String> attd = new ArrayList<>();

    public static void main(String argv[]) throws Exception {
//        Set server socket to port 5000
        welcomeSocket = new ServerSocket(5000);
        
//        new EmailSend().send("EC4421");
//      Listen to connections
        while (true) {

            String way = recieveData();
            System.out.println(way);

            if (way.equals("f")) {
                subCode = recieveData();
                String fpID = recieveData();
                System.out.println(subCode);
                System.out.println(fpID);

                getStudentfp(subCode, fpID);

                System.out.println(studID);
                System.out.println(studName);

                attd.add(studID + " FPS");

                sendData("y," + studID + "," + studName + ",\n");
            } else if (way.equals("b")) {

                subCode = recieveData();
                String fpID = recieveData();
                String magID = recieveData();
                System.out.println(subCode);
                System.out.println(fpID);
                System.out.println(magID);

                getStudentboth(subCode, fpID, magID);

                System.out.println(studID);
                System.out.println(studName);

                attd.add(studID + " FPS & Card");

                sendData("y," + studID + "," + studName + ",\n");

            } else if(way.equals("k")){
                System.out.println("write");
                writeFile();
            }

        }

    }

//    Receive data through port 5000 when socket connected
    static String recieveData() throws Exception {
        String data;
        Socket consoc = welcomeSocket.accept();
        System.out.println("got");
        BufferedReader inFromDevice = new BufferedReader(new InputStreamReader(consoc.getInputStream()));
        data = inFromDevice.readLine();
        consoc.close();
        return data;
    }
    
//  Send data through port 5000 when socket connected
    static void sendData(String student) throws Exception {
        Socket consoc = welcomeSocket.accept();
        BufferedWriter outToDevice = new BufferedWriter(new OutputStreamWriter(consoc.getOutputStream()));
        outToDevice.write(student);
        outToDevice.flush();
        consoc.close();
    }

//    Get student from data base when finger-print only option choosed
    public static void getStudentfp(String subID, String fpID) throws Exception {

//        Get data to a resulset from data base, SQL query written
        ResultSet rset = new jdb().getdata("SELECT s.Student_ID, s.Student_Name FROM student s, students_courses c "
                + "WHERE s.Student_ID=c.Student_ID AND c.Subject_ID='" + subID + "' AND s.Finger_ID='" + fpID + "'");

//        Set student details
        while (rset.next()) {
            studID = rset.getString(1);
            studName = rset.getString(2);
        }

    }

//    Get student details from databse when bothways option choosed
    public static void getStudentboth(String subID, String fpID, String magID) throws Exception {

//        Get data to a resulset from data base, SQL query written        
        ResultSet rset = new jdb().getdata("SELECT s.Student_ID, s.Student_Name FROM student s, students_courses c "
                + "WHERE s.Student_ID=c.Student_ID AND c.Subject_ID='" + subID + "' AND s.Finger_ID='" + fpID + "' "
                + "AND s.Card_ID='" + magID + "'");

        //        Set student details
        while (rset.next()) {
            studID = rset.getString(1);
            studName = rset.getString(2);
        }

    }

//    write the final details into a file
    public static void writeFile() throws Exception{
        File file = new File("ATTD");
        PrintWriter writer = new PrintWriter(file);
        for (String string : attd) {
            writer.println(string);
        }
        writer.close();
//        send file to defined email
        new EmailSend().send(subCode,"wageesha.2012@gmail.com");
//        sendData("OK");
    }
}
