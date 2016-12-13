package android_project.com.doctorassistant;

import android.os.AsyncTask;
import android.util.Log;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;
import org.apache.http.util.EntityUtils;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

//import org.apache.http.entity.mime.HttpMultipartMode;
//import org.apache.http.entity.mime.MultipartEntity;
//import org.apache.http.entity.mime.content.FileBody;

/**
 * Created by Tejas on 4/25/15.
 */


public class databasehelper{
    String name1;
    String email1;
    String id1;
    String pswd1;
    String add1;
    String cont1;
    String pgender1;
    String dob1;
    String notes1;

    public databasehelper(String name, String email, String id,String pswd)
    {
        name1 = name;
        email1 = email;
        id1 = id;
        pswd1 =pswd;
    }

    public databasehelper(String name, String address, String contact, String email,String gender, String dob, String notes)
    {
        name1 = name;
        add1 = address;
        cont1 = contact;
        email1 = email;
        pgender1 = gender;
        dob1 = dob;
        notes1 = notes;

    }

//    public databasehelper(registration registration) {
//
//    }

    public class PostDataAsyncTask extends AsyncTask<String, String, String> {

        @Override
        protected String doInBackground(String... strings) {
            try{

                // url where the data will be posted
                String postReceiverUrl = "http://52.24.51.46/phpFiles/register.php";
                //Log.v(TAG, "postURL: " + postReceiverUrl);

                // HttpClient
                HttpClient httpClient = new DefaultHttpClient();

                // post header
                HttpPost httpPost = new HttpPost(postReceiverUrl);

              //  Log.d("fields",name1+email1+id1+pswd1);

                // add your data
                List<NameValuePair> nameValuePairs = new ArrayList<NameValuePair>(2);
                nameValuePairs.add(new BasicNameValuePair("name", name1));
                nameValuePairs.add(new BasicNameValuePair("email", email1));
                nameValuePairs.add(new BasicNameValuePair("id", id1));
                nameValuePairs.add(new BasicNameValuePair("pswd", pswd1));



                httpPost.setEntity(new UrlEncodedFormEntity(nameValuePairs));

                // execute HTTP post request
                HttpResponse response = httpClient.execute(httpPost);
                HttpEntity resEntity = response.getEntity();

                if (resEntity != null) {

                    final String responseStr = EntityUtils.toString(resEntity).trim();
                    //   Log.v(TAG, "Response: " +  responseStr);
//                    activity.runOnUiThread(new Runnable() {
//                        public void run() {
//                            Toast.makeText(activity, regId + responseStr, Toast.LENGTH_SHORT).show();
//                        }
//                    });

                    // you can add an if statement here and do other actions based on the response
                }

            } catch (ClientProtocolException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            }
            return null;

        }

        protected void onPreExecute() {
            super.onPreExecute();
            // do stuff before posting data
        }
    }

    public class PostDataAsyncTask1 extends AsyncTask<String, String, String> {


        private static final String TAG ="" ;

        @Override
        protected String doInBackground(String... strings) {
            try{
                // url where the data will be posted
                String postReceiverUrl = "http://52.24.51.46/phpFiles/Add_patient.php";
                Log.v(TAG, "postURL: " + postReceiverUrl);

                // HttpClient
                HttpClient httpClient = new DefaultHttpClient();

                // post header
                HttpPost httpPost = new HttpPost(postReceiverUrl);

                  Log.d("fields", name1 + add1 + cont1 + email1 + pgender1 + dob1 + notes1);

                // add your data
                List<NameValuePair> nameValuePairs = new ArrayList<NameValuePair>(2);
                nameValuePairs.add(new BasicNameValuePair("pname", name1));
                nameValuePairs.add(new BasicNameValuePair("paddress", add1));
                nameValuePairs.add(new BasicNameValuePair("pcontact", cont1));
                nameValuePairs.add(new BasicNameValuePair("pemail", email1));
                nameValuePairs.add(new BasicNameValuePair("ppgender", pgender1));
                nameValuePairs.add(new BasicNameValuePair("pdob", dob1));
                nameValuePairs.add(new BasicNameValuePair("notes", notes1));


                httpPost.setEntity(new UrlEncodedFormEntity(nameValuePairs));

                // execute HTTP post request
                HttpResponse response = httpClient.execute(httpPost);
                HttpEntity resEntity = response.getEntity();

                if (resEntity != null) {

                    final String responseStr = EntityUtils.toString(resEntity).trim();
                    //   Log.v(TAG, "Response: " +  responseStr);
//                    activity.runOnUiThread(new Runnable() {
//                        public void run() {

//                        Toast.makeText(activity, regId + responseStr, Toast.LENGTH_SHORT).show();
//                        }
//                    });

                    // you can add an if statement here and do other actions based on the response
                }

            } catch (ClientProtocolException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            }
            return null;

        }

        protected void onPreExecute() {
            super.onPreExecute();
            // do stuff before posting data
        }
    }

    public void insert(){
        new PostDataAsyncTask().execute();
    }

    public void insert_patient(){
        new PostDataAsyncTask1().execute();
    }
    public void authentication()
    {


    }

}









//public class databasehelper extends SQLiteOpenHelper {
//
//    private static final int DATABASE_VERSION = 1;
//    private static final String DATABASE_NAME = "docinfo.db";
//
//    private static final String TABLE_NAME = "docinfo";
//    private static final String COLUMN_ID = "id";
//    private static final String COLUMN_name = "name";
//    private static final String COLUMN_email = "email";
//    private static final String COLUMN_username = "username";
//    private static final String COLUMN_password = "password";
//    SQLiteDatabase db;
//
//
//
//    private static final String TABLE_CREATE = "create table docinfo (id integer primary key not null ,"+
//            "name text not null ,email text not null ,username text not null ,password text not null);";
//
//
//
//
//    public databasehelper(Context context) {
//        super(context, DATABASE_NAME , null, DATABASE_VERSION);
//    }
//
//
//
//    @Override
//    public void onCreate(SQLiteDatabase db) {
//
//        db.execSQL(TABLE_CREATE);
//        this.db = db;
//
//
//    }
//
//    public void insert(userinfo c){
//
//        db = getWritableDatabase();
//        ContentValues values = new ContentValues();
//
//        String query = "select * from docinfo";
//        Cursor cursor = db.rawQuery(query, null);
//
//        int count = cursor.getCount();
//
//        values.put(COLUMN_ID, count);
//
//        values.put(COLUMN_name, c.getName());
//        values.put(COLUMN_email, c.getEmail());
//        values.put(COLUMN_username, c.getUsername());
//        values.put(COLUMN_password, c.getPassword());
//
//        db.insert(TABLE_NAME, null, values);
//    }
//
//    public String searchPass(String username)
//    {
//        db = this.getReadableDatabase();
//        String query = " select username, password from "+TABLE_NAME;
//        Cursor cursor = db.rawQuery(query, null);
//        String a, b;
//        b = "not found";
//        if(cursor.moveToFirst())
//        {
//            do{
//                a=cursor.getString((0));
//
//                if(a.equals(username))
//                {
//                    b = cursor.getString(1);
//                    break;
//                }
//            }
//            while(cursor.moveToNext());
//        }
//       return b;
//
//    }
//    @Override
//    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
//    String query = "DROP TABLE IF EXISTS" +TABLE_NAME;
//        db.execSQL(query);
//        this.onCreate(db);
//
//    }
//}
