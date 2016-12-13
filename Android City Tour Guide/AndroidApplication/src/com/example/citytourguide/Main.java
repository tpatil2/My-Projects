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
import android.os.Bundle;
import android.app.Activity;
import android.content.Intent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.support.v4.app.NavUtils;

public class Main extends Activity {
	ArrayAdapter<String> listAdapter;
	Vector<SingleComment> allcomments;
	ListView list;
	Button search;
	Button logout;
	SingleUser su;



	private static final String NAMESPACE = "http://ServerPack/";
	private static final String URL = "http://10.0.2.2:8080/CityTourGuideServer/CityTourGuideWS?wsdl";//"http://localhost:8080/WebApplication1/NewWebService?wsdl";	
	private static final String SOAP_ACTION = "CityTourGuideWS";


	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		Bundle b = getIntent().getExtras();
		if(b!=null){
			su=(SingleUser)b.getSerializable("su");
		}

		list=(ListView)findViewById(R.id.listViewRecent);
		listAdapter= new ArrayAdapter<String>(this,android.R.layout.simple_list_item_1);
		allcomments=(Vector<SingleComment>)callService("", "getRecentComments", "input");
		search=(Button)findViewById(R.id.buttonSearch);
		logout=(Button)findViewById(R.id.butLogout);
		fillList();

		search.setOnClickListener(new View.OnClickListener() {

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				Intent i = new Intent(v.getContext(),SearchByArea.class);
				i.putExtra("su", su);
				startActivity(i);
			}
		});
		
		
		logout.setOnClickListener(new View.OnClickListener() {

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				Intent i = new Intent(v.getContext(),Login.class);
				finish();
				
				
				startActivity(i);
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

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		getMenuInflater().inflate(R.menu.activity_main, menu);
		return true;
	}
	void fillList(){
		listAdapter.clear();
		if(allcomments.size()>=5){
			for(int i =0;i<=4;i++){
				
				String temp=allcomments.elementAt(i).comments;
				if(temp.length()<=10){
					listAdapter.add(temp);

				}else{
					listAdapter.add(temp.substring(0,10));
				}
			}
		}else{
			for(int i =0;i<allcomments.size();i++){
				String temp=allcomments.elementAt(i).comments;
				if(temp.length()<=10){
					listAdapter.add(temp);

				}else{
					listAdapter.add(temp.substring(0,10));
				}
			}
		}
		list.setAdapter(listAdapter);
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
