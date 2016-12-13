package android_project.com.doctorassistant;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;

/**
 * Created by RAHUL on 4/25/2015.
 */
public class Patient_Search extends Activity implements View.OnClickListener{

Button b1;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.patient_search);


        b1 = (Button)findViewById(R.id.patient1_search);
        b1.setOnClickListener(this);

    }



    @Override
    public void onClick(View v) {


        if (v.getId() == R.id.patient1_search) {
            Intent i = new Intent(Patient_Search.this, Patient_Info.class);
            startActivity(i);
        }

    }
}

