/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package ServerPack;

import MyPack.SingleCategory;
import MyPack.SingleComment;
import MyPack.SingleLocation;
import MyPack.SingleUser;
import java.io.*;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.Statement;
import java.util.Stack;
import java.util.Vector;
import javax.jws.WebService;
import javax.jws.WebMethod;
import javax.jws.WebParam;

/**
 *
 * @author Ravee
 */
@WebService(serviceName = "CityTourGuideWS")
public class CityTourGuideWS {

    Connection con;
    String ssql;
    ResultSet rs;
    Statement stmt;
    SingleUser su;
    SingleComment sc;
    SingleCategory cat;
    SingleLocation sl;
    Vector<SingleComment> recentComments;
    Vector<SingleCategory> allCategorys;
    Vector<SingleLocation> allLocations;
    Vector<SingleComment> allComments;

    @WebMethod(operationName = "hello")
    public String hello(@WebParam(name = "name") String txt) {
        return "Hello " + txt + " !";
    }

    Object stringToObject(String inp) {
        byte b[] = Base64.decode(inp);
        Object ret = null;
        try {
            ByteArrayInputStream bis = new ByteArrayInputStream(b);
            ObjectInput in = new ObjectInputStream(bis);
            ret = (Object) in.readObject();
            bis.close();
            in.close();
        } catch (Exception e) {
            System.out.println("NOT DE-SERIALIZABLE: " + e);
        }
        return ret;
    }

    String objectToString(Object obj) {
        byte[] b = null;
        try {
            ByteArrayOutputStream bos = new ByteArrayOutputStream();
            ObjectOutput out = new ObjectOutputStream(bos);
            out.writeObject(obj);
            b = bos.toByteArray();
        } catch (Exception e) {
            System.out.println("NOT SERIALIZABLE: " + e);
        }
        return Base64.encode(b);
    }

    void getConnection() {
        String connection = "jdbc:mysql://localhost/db5775";
        String user = "root";
        String password = "krishna";

        String ssql;
        try {
            Class.forName("com.mysql.jdbc.Driver").newInstance();
            con = DriverManager.getConnection(connection, user, password);
            System.out.println("Database Connection OK");
        } catch (Exception e) {
            System.out.println("Error opening database : " + e);
        }
    }

    @WebMethod(operationName = "isUIDAvailable")
    public String isUIDAvailable(@WebParam(name = "username") String username) {
        //TODO write your implementation code here:
        boolean flag = false;
        getConnection();
        try {
            ssql = "select * from user where uid='" + username + "'";
            System.out.println("QUERY :" + ssql);
            stmt = con.createStatement(ResultSet.CONCUR_UPDATABLE, ResultSet.TYPE_SCROLL_INSENSITIVE);
            rs = stmt.executeQuery(ssql);
            if (rs.next()) {
                return objectToString(false);
            }
        } catch (Exception e) {
            System.out.println("ERROR :" + e);
        }


        return objectToString(true);
    }

    @WebMethod(operationName = "signUp")
    public String signUp(@WebParam(name = "input") String input) {
        //TODO write your implementation code here:
        boolean flag = false;
        getConnection();
        su = (SingleUser) stringToObject(input);
        try {
            ssql = "insert into user values('" + su.uid + "','" + su.uName + "','" + su.password + "','" + su.email + "','" + su.contact + "')";
            System.out.println("QUERY :" + ssql);
            stmt = con.createStatement(ResultSet.CONCUR_UPDATABLE, ResultSet.TYPE_SCROLL_INSENSITIVE);
            stmt.executeUpdate(ssql);
            flag = true;
        } catch (Exception e) {
            System.out.println("ERROR :" + e);
        }
        return objectToString(flag);
    }

    @WebMethod(operationName = "login")
    public String login(@WebParam(name = "input") String input) {
        //TODO write your implementation code here:
        boolean flag = false;
        getConnection();
        su = (SingleUser) stringToObject(input);
        try {
            ssql = "select * from user where uid='" + su.uid + "' and password='" + su.password + "'";
            System.out.println("QUERY :" + ssql);
            stmt = con.createStatement(ResultSet.CONCUR_UPDATABLE, ResultSet.TYPE_SCROLL_INSENSITIVE);
            rs = stmt.executeQuery(ssql);
            su = new SingleUser();
            if (rs.next()) {
                su.uid = rs.getString(1);
                su.uName = rs.getString(2);
                su.password = rs.getString(3);
                su.email = rs.getString(4);
                su.contact = rs.getString(5);
            }
        } catch (Exception e) {
            System.out.println("ERROR :" + e);
        }
        return objectToString(su);
    }

    @WebMethod(operationName = "getRecentComments")
    public String getRecentComments(@WebParam(name = "input") String input) {
        //TODO write your implementation code here:
        boolean flag = false;
        getConnection();
        try {
            ssql = "select * from comments order by cdate ";
            System.out.println("QUERY :" + ssql);
            stmt = con.createStatement(ResultSet.CONCUR_UPDATABLE, ResultSet.TYPE_SCROLL_INSENSITIVE);
            rs = stmt.executeQuery(ssql);
            recentComments = new Stack<SingleComment>();
            while (rs.next()) {
                sc = new SingleComment();
                sc.cid = rs.getInt(1);
                sc.tid = rs.getInt(2);
                sc.uid = rs.getString(3);
                sc.email = rs.getString(4);
                sc.mobile = rs.getString(5);
                sc.comments = rs.getString(6);
                sc.rating = rs.getFloat(7);
                sc.cdate = rs.getString(8);
                recentComments.add(sc);
            }
        } catch (Exception e) {
            System.out.println("ERROR :" + e);
        }
        return objectToString(recentComments);
    }

    @WebMethod(operationName = "getCategory")
    public String getCategory(@WebParam(name = "input") String input) {
        //TODO write your implementation code here:
        boolean flag = false;
        getConnection();
        try {
            ssql = "select * from categories ";
            System.out.println("QUERY :" + ssql);
            stmt = con.createStatement(ResultSet.CONCUR_UPDATABLE, ResultSet.TYPE_SCROLL_INSENSITIVE);
            rs = stmt.executeQuery(ssql);
            allCategorys = new Stack<SingleCategory>();
            while (rs.next()) {
                cat = new SingleCategory();
                cat.catName = rs.getString(1);
                allCategorys.add(cat);
            }
        } catch (Exception e) {
            System.out.println("ERROR :" + e);
        }
        return objectToString(allCategorys);
    }

    int[] getIntArray(byte[] arr) {
        int[] ret = new int[arr.length / 4];

        for (int i = 0, j = 0; i < ret.length; i++, j += 4) {
            ret[i] = arr[j] | (arr[j + 1] << 8) | (arr[j + 2] << 16) | (arr[j + 3] << 24);
        }

        return ret;
    }

    @WebMethod(operationName = "searchByArea")
    public String searchByArea(@WebParam(name = "input") String input) {
        //TODO write your implementation code here:
        boolean flag = false;
        sl = (SingleLocation) stringToObject(input);
        getConnection();
        try {
            ssql = "select * from location where category='" + sl.category + "' and area='" + sl.area.areaName + "'";
            System.out.println("QUERY :" + ssql);
            stmt = con.createStatement(ResultSet.CONCUR_UPDATABLE, ResultSet.TYPE_SCROLL_INSENSITIVE);
            rs = stmt.executeQuery(ssql);
            allLocations = new Stack<SingleLocation>();
            while (rs.next()) {
                sl = new SingleLocation();
                sl.category = rs.getString("category");
                sl.area.areaName = rs.getString("area");
                sl.description = rs.getString("description");
                sl.name = rs.getString("locationname");
                sl.image = getIntArray(rs.getBytes("image"));
                System.out.println("Leng " + sl.image.length);
                sl.tid = rs.getInt("tid");
                allLocations.add(sl);
            }
        } catch (Exception e) {
            System.out.println("ERROR :" + e);
        }
        return objectToString(allLocations);
    }

    @WebMethod(operationName = "getComments")
    public String getComments(@WebParam(name = "input") String input) {
        //TODO write your implementation code here:
        boolean flag = false;
        getConnection();
        try {
            ssql = "select * from comments where tid=" + input + " order by cdate";
            System.out.println("QUERY :" + ssql);
            stmt = con.createStatement(ResultSet.CONCUR_UPDATABLE, ResultSet.TYPE_SCROLL_INSENSITIVE);
            rs = stmt.executeQuery(ssql);
            allComments = new Stack<SingleComment>();
            while (rs.next()) {
                sc = new SingleComment();
                sc.cid = rs.getInt(1);
                sc.tid = rs.getInt(2);
                sc.uid = rs.getString(3);
                sc.email = rs.getString(4);
                sc.mobile = rs.getString(5);
                sc.comments = rs.getString(6);
                sc.rating = rs.getFloat(7);
                sc.cdate = rs.getString(8);
                allComments.add(sc);
            }
        } catch (Exception e) {
            System.out.println("ERROR :" + e);
        }
        return objectToString(allComments);
    }

    @WebMethod(operationName = "addComments")
    public String addComments(@WebParam(name = "input") String input) {
        //TODO write your implementation code here:
        boolean flag = false;
        getConnection();
        sc = (SingleComment) stringToObject(input);
        try {
            ssql = "insert into comments values(" + getNextID("comments", "cid") + "," + sc.tid + ",'" + sc.uid + "','" + sc.email + "','" + sc.mobile + "','" + sc.comments + "','" + sc.rating + "', sysdate())";
            System.out.println("QUERY :" + ssql);
            stmt = con.createStatement(ResultSet.CONCUR_UPDATABLE, ResultSet.TYPE_SCROLL_INSENSITIVE);
            stmt.executeUpdate(ssql);
            flag = true;
        } catch (Exception e) {
            System.out.println("ERROR :" + e);
        }
        return objectToString(flag);
    }

    public int getNextID(String tableName, String field) {
        int ret = 0;
        try {
            Statement st = con.createStatement(ResultSet.TYPE_SCROLL_INSENSITIVE, ResultSet.CONCUR_UPDATABLE);
            ResultSet rs = st.executeQuery("select max(" + field + ") from " + tableName);
            if (rs.next()) {
                ret = rs.getInt(1);
            }
        } catch (Exception e) {
            System.out.println("Error in getNextID : " + e);
        }
        ret++;
        return ret;
    }
}
