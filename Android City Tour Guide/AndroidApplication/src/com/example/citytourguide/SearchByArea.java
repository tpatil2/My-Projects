package com.example.citytourguide;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.ObjectInput;
import java.io.ObjectInputStream;
import java.io.ObjectOutput;
import java.io.ObjectOutputStream;
import java.util.Vector;

import org.apache.http.impl.conn.SingleClientConnManager;
import org.ksoap2.SoapEnvelope;
import org.ksoap2.serialization.PropertyInfo;
import org.ksoap2.serialization.SoapObject;
import org.ksoap2.serialization.SoapSerializationEnvelope;
import org.ksoap2.transport.HttpTransportSE;

import MyPack.SingleCategory;
import MyPack.SingleComment;
import MyPack.SingleLocation;
import MyPack.SingleUser;
import android.R.string;
import android.os.Bundle;
import android.app.Activity;
import android.content.Intent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.Spinner;
import android.support.v4.app.NavUtils;

public class SearchByArea extends Activity {

	ArrayAdapter<String> listAdapter;
	ArrayAdapter<String> spinnerAdapter;
	Vector<SingleLocation> allLocation;
	ListView list;
	Button search;
	SingleUser su;
	Spinner category;
	Vector<SingleCategory> allCategorys;
	EditText area;
	SingleLocation sl;



	private static final String NAMESPACE = "http://ServerPack/";
	private static final String URL = "http://10.0.2.2:8080/CityTourGuideServer/CityTourGuideWS?wsdl";//"http://localhost:8080/WebApplication1/NewWebService?wsdl";	
	private static final String SOAP_ACTION = "CityTourGuideWS";


	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_search_by_area);

		Bundle b = getIntent().getExtras();
		if(b!=null){
			su=(SingleUser)b.getSerializable("su");
		}

		list=(ListView)findViewById(R.id.listComments);
		area=(EditText)findViewById(R.id.editTextArea);

		listAdapter= new ArrayAdapter<String>(this,android.R.layout.simple_list_item_1);
		spinnerAdapter= new ArrayAdapter<String>(this,android.R.layout.simple_spinner_dropdown_item);
		category=(Spinner)findViewById(R.id.spinnerCat);



		allCategorys=(Vector<SingleCategory>)callService("", "getCategory", "input");

		search=(Button)findViewById(R.id.buttonSearchComm);
		list.setOnItemLongClickListener(new AdapterView.OnItemLongClickListener() {
			@Override
			public boolean onItemLongClick(AdapterView<?> arg0, View arg1, int arg2,
					long arg3) {
				// TODO Auto-generated method stub
				Intent i = new Intent(arg1.getContext(),ShowCommentsList.class);
				i.putExtra("tid",""+ allLocation.elementAt(arg2).tid);
				i.putExtra("su", su);
				startActivity(i);
				return false;
			}
		});


		search.setOnClickListener(new View.OnClickListener() {

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				sl = new SingleLocation();
				sl.area.areaName=area.getText().toString();
				sl.category=category.getSelectedItem().toString();
				allLocation=(Vector<SingleLocation>)callService(objectToString(sl), "searchByArea", "input");
				fillList();

			}
		});
		fillCombo();
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


	void fillList(){
			
		
		list.setAdapter(new MyArrayAdapter(this, new String[allLocation.size()], allLocation));
	}


	void fillCombo(){
		spinnerAdapter.clear();

		for(int i =0;i<allCategorys.size();i++){
			spinnerAdapter.add(allCategorys.elementAt(i).catName);
		}
		category.setAdapter(spinnerAdapter);
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
		getMenuInflater().inflate(R.menu.activity_search_by_area, menu);
		return true;
	}


}
