package com.example.citytourguide;

import java.util.Vector;

import MyPack.SingleLocation;
import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.TextView;

public class MyArrayAdapter extends ArrayAdapter<String> {
	 private final Activity context;
	 private final String[] loc;
	 
	 
	 private final Vector<SingleLocation> allLoc ;
	 
	 public MyArrayAdapter(Activity context, String[] loc,Vector<SingleLocation> allLoc) {
		 super(context, R.layout.customlist, loc);
		 this.context = context;
		 this.loc = loc;
		 this.allLoc=allLoc;
	 }
	 
	 // static to save the reference to the outer class and to avoid access to
	 // any members of the containing class
	 static class ViewHolder {
		 public ImageView imageView;
		 public TextView textView;
		 public TextView textView2;
	 }
	 
	 @Override
	 public View getView(int position, View convertView, ViewGroup parent) {
		 // ViewHolder will buffer the assess to the individual fields of the row
		 // layout
	 
		 ViewHolder holder;
		 // Recycle existing view if passed as parameter
		 // This will save memory and time on Android
		 // This only works if the base layout for all classes are the same
		 View rowView = convertView;
		 if (rowView == null) {
			 LayoutInflater inflater = context.getLayoutInflater();
			 rowView = inflater.inflate(R.layout.customlist, null, true);
			 holder = new ViewHolder();
	 
			 holder.textView = (TextView) rowView.findViewById(R.id.LocName);
			 holder.textView2 = (TextView) rowView.findViewById(R.id.tvDesc);
			 holder.imageView = (ImageView) rowView.findViewById(R.id.ImageView01);
			 rowView.setTag(holder);
		 } else {
			 holder = (ViewHolder) rowView.getTag();
		 }
	 
		 holder.textView.setText(allLoc.elementAt(position).name);
		 holder.textView2.setText(allLoc.elementAt(position).description);
		 // Change the icon for Windows and iPhone
		 Bitmap b = Bitmap.createBitmap(allLoc.elementAt(position).image, 100, 80, Config.RGB_565);
		 holder.imageView.setImageBitmap(b);
		 //holder.imageView.setImageDrawable(LoadImageFromWebOperations(link[position]));//LoadImageFromWebOperations((link[position]));
		//here is set the image
		//you can write like this: holder.imageView.setImageResource(R.drawable.myimage);
		 //instead i have used function LoadImageFromWebOperations which recieve the link
		//to the image like parameter
		//and returns a drawable
	 
		 return rowView;
	 }
}