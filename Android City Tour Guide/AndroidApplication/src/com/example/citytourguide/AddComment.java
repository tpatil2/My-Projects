package com.example.citytourguide;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.ObjectInput;
import java.io.ObjectInputStream;
import java.io.ObjectOutput;
import java.io.ObjectOutputStream;
import java.util.Vector;

import org.ksoap2.SoapEnvelope;
import org.ksoap2.serialization.PropertyInfo;
import org.ksoap2.serialization.SoapObject;
import org.ksoap2.serialization.SoapSerializationEnvelope;
import org.ksoap2.transport.HttpTransportSE;

import MyPack.SingleComment;
import MyPack.SingleUser;
import android.R.integer;
import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.RatingBar;
import android.widget.Toast;
import android.support.v4.app.NavUtils;

public class AddComment extends Activity {


	SingleUser su;
	String tid;
	Button add;
	EditText comment;
	SingleComment sc;
	RatingBar rate;



	private static final String NAMESPACE = "http://ServerPack/";
	private static final String URL = "http://10.0.2.2:8080/CityTourGuideServer/CityTourGuideWS?wsdl";//"http://localhost:8080/WebApplication1/NewWebService?wsdl";	
	private static final String SOAP_ACTION = "CityTourGuideWS";



	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_add_comment);
		Bundle b = getIntent().getExtras();
		if(b!=null){
			su=(SingleUser)b.getSerializable("su");
			tid=b.getString("tid");
			System.out.println("GOT UID :"+su.uid);
		}
		comment=(EditText)findViewById(R.id.editTextComment);
		rate=(RatingBar)findViewById(R.id.ratingBar1);
		add=(Button)findViewById(R.id.butDone);
		add.setOnClickListener(new View.OnClickListener() {

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				sc = new SingleComment();
				sc.tid=Integer.parseInt(tid);
				sc.uid=su.uid;
				sc.email=su.email;
				sc.mobile=su.contact;
				sc.comments=comment.getText().toString();
				sc.rating=rate.getRating();


				boolean b = (Boolean)callService(objectToString(sc), "addComments", "input");
				if(b){
					Toast.makeText(v.getContext(), "Your Comment Added Successfully", Toast.LENGTH_SHORT).show();
				}else{
					Toast.makeText(v.getContext(), "Please Verify The connection cannot add comment", Toast.LENGTH_SHORT).show();
				}


			}
		});


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

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		getMenuInflater().inflate(R.menu.activity_add_comment, menu);
		return true;
	}


}
