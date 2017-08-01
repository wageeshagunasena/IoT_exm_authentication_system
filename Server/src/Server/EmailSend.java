/*
 * The mail class.
 * This is used to send the email to the defined email address
 * 
 */
package Server;

import java.io.File;
import java.util.Properties;
import javax.activation.DataHandler;
import javax.activation.DataSource;
import javax.activation.FileDataSource;
import javax.mail.Message;
import javax.mail.MessagingException;
import javax.mail.Multipart;
import javax.mail.Session;
import javax.mail.Transport;
import javax.mail.internet.InternetAddress;
import javax.mail.internet.MimeBodyPart;
import javax.mail.internet.MimeMessage;
import javax.mail.internet.MimeMultipart;

/**
 *
 * @author WAGEESHA-PC
 */
public class EmailSend {

    public void send(String subject,String email) {

        try {
//            Set credentials
            String host = "smtp.gmail.com";
            String user = "wagguna.123@gmail.com";
            String pass = "0702017667";
            String to = email;
            String from = "wagguna.123@gmail.com";

            boolean sessionDebug = false;

            Properties props = System.getProperties();
//          Set smtp properties
            props.put("mail.smtp.starttls.enable", "true");
            props.put("mail.smtp.host", host);
            props.put("mail.smtp.port", 587);
            props.put("mail.smtp.auth", "true");
            props.put("mail.smtp.starttls.required", "true");

            java.security.Security.addProvider(new com.sun.net.ssl.internal.ssl.Provider());
            Session mailSession = Session.getDefaultInstance(props, null);
            mailSession.setDebug(sessionDebug);
            Message msg = new MimeMessage(mailSession);
            msg.setFrom(new InternetAddress(from));
            InternetAddress[] address = {new InternetAddress(to)};
            msg.setRecipients(Message.RecipientType.TO, address);
            msg.setSubject("Attendance list of "+subject);
            msg.setText("");

            Multipart multipart = new MimeMultipart();

            MimeBodyPart textBodyPart = new MimeBodyPart();
            textBodyPart.setText("");

//            Attach file to email
            MimeBodyPart attachmentBodyPart = new MimeBodyPart();
            File attachementPath;
            DataSource source = new FileDataSource("ATTD"); // ex : "C:\\test.pdf"
            attachmentBodyPart.setDataHandler(new DataHandler(source));
            attachmentBodyPart.setFileName("ATTD"); // ex : "test.pdf"

            multipart.addBodyPart(textBodyPart);  // add the text part
            multipart.addBodyPart(attachmentBodyPart); // add the attachement part

            msg.setContent(multipart);

//            Send Email
            Transport transport = mailSession.getTransport("smtp");
            transport.connect(host, user, pass);
            transport.sendMessage(msg, msg.getAllRecipients());
            transport.close();
//            Transport.send(msg);
            System.out.println("Message send");
        } catch (MessagingException messagingException) {
            messagingException.printStackTrace();
        }

    }

}
