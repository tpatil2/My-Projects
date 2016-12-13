package com.example.citytourguide;



import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.ObjectInput;
import java.io.ObjectInputStream;
import java.io.ObjectOutput;
import java.io.ObjectOutputStream;
import java.security.MessageDigest;

import org.ksoap2.SoapEnvelope;
import org.ksoap2.serialization.PropertyInfo;
import org.ksoap2.serialization.SoapObject;
import org.ksoap2.serialization.SoapSerializationEnvelope;
import org.ksoap2.transport.HttpTransportSE;



import MyPack.SingleUser;
import android.os.Bundle;
import android.app.Activity;
import android.content.Intent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;
import android.support.v4.app.NavUtils;

public class Login extends Activity {
	Button login;
	TextView username;
	TextView password;
	Button sign;
	SingleUser su;



	private static final String NAMESPACE = "http://ServerPack/";
	private static final String URL = "http://10.0.2.2:8080/CityTourGuideServer/CityTourGuideWS?wsdl";//"http://localhost:8080/WebApplication1/NewWebService?wsdl";	
	private static final String SOAP_ACTION = "CityTourGuideWS";

	@Override



	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_login);
		login=(Button)findViewById(R.id.butLogin);
		sign=(Button)findViewById(R.id.butSignUp);
		username=(TextView)findViewById(R.id.editText1);
		password=(TextView)findViewById(R.id.editText2);
		login.setOnClickListener(new View.OnClickListener() {

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				su = new SingleUser();
				su.uid=username.getText().toString();
				su.password=getSHA(password.getText().toString());
				su =(SingleUser)  callService(objectToString(su), "login", "input");
				System.out.println("Responser After Conversion : " + su.uName);
				if(su.uName.equalsIgnoreCase("")){
					Toast.makeText(v.getContext(), "Please Verify Username Or Password ", Toast.LENGTH_LONG).show();

				}else{
					Toast.makeText(v.getContext(), "Login Successful", Toast.LENGTH_SHORT).show();
					Intent i = new Intent(v.getContext(),Main.class);
					
					i.putExtra("su", su);
					finish();
					startActivity(i);

				}

			}
		});

		sign.setOnClickListener(new View.OnClickListener() {

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				Intent i = new Intent(v.getContext(),SignUp.class);
				//finish();
				startActivity(i);




			}
		});


	}
	
	  String getSHA(String str) {
	        try {
	            MessageDigest md = MessageDigest.getInstance("SHA-1");
	            md.update(str.getBytes("iso-8859-1"), 0, str.length());
	            byte[] sha1hash = md.digest();
	            int v1=0;
	            StringBuilder sb = new StringBuilder();
	            String out = "";
	            for (int i = 0; i < sha1hash.length; i++) {
	                v1 = sha1hash[i] & 0x0f;
	                if(v1 < 10){
	                    sb.append((char)(v1 + 48));
	                }else{
	                    sb.append((char)(v1 + 55));
	                }
	                
	                v1 = (sha1hash[i] >> 4) & 0x0f;
	                if(v1 < 10){
	                    sb.append((char)(v1 + 48));
	                }else{
	                    sb.append((char)(v1 + 55));
	                }
	            }
	            out = sb.toString();
	            System.out.println(out);
	            return (out);
	        } catch (Exception e) {
	            // TODO: handle exception
	            System.out.println("ERROR :"+e);
	            e.printStackTrace();
	        }
	        return "";
	    }

	Object callService(String INPUT_DATA, String METHOD_NAME, String PARAMETER_NAME){

		SoapObject request = new SoapObject(NAMESPACE, METHOD_NAME);

		SoapSerializationEnvelope envelope = new SoapSerializationEnvelope(SoapEnvelope.VER11); 

		PropertyInfo pi = new PropertyInfo();
		pi.setName(PARAMETER_NAME);
		pi.setValue(INPUT_DATA);
		pi.setType(String.class);

		request.addProperty(pi);

		envelope.setOutputSoapObject(request);

		HttpTransportSE androidHttpTransport = new HttpTransportSE(URL);

		try {
			androidHttpTransport.call(SOAP_ACTION, envelope);

			SoapObject resultsRequestSOAP = (SoapObject)envelope.bodyIn;

			String resp = resultsRequestSOAP.getPrimitivePropertyAsString("return");

			return stringToObject(resp);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}

	Object stringToObject(String inp){
		byte b[] = Base64.decode(inp);
		Object ret = null;
		try {
			ByteArrayInputStream bis = new ByteArrayInputStream(b);
			ObjectInput in = new ObjectInputStream(bis);
			ret = (Object) in.readObject(); 
			bis.close();
			in.close();
		} catch(Exception e) {
			System.out.println("NOT DE-SERIALIZABLE: " + e);
		}
		return ret;
	}

	String objectToString(Object obj){
		byte[] b = null;
		try {
			ByteArrayOutputStream bos = new ByteArrayOutputStream();
			ObjectOutput out = new ObjectOutputStream(bos);   
			out.writeObject(obj);
			b = bos.toByteArray();
		} catch(Exception e) {
			System.out.println("NOT SERIALIZABLE: " + e);
		}         
		return Base64.encode(b);
	}




}
