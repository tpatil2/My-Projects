package android_project.com.doctorassistant;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

/**
 * Created by RAHUL on 4/25/2015.
 */
public class Billing extends Activity implements View.OnClickListener{

    Button b1;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.billing);


        b1 = (Button)findViewById(R.id.show_bill);
        b1.setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {

        if(v.getId()==R.id.show_bill)
        {
            Intent i = new Intent(Billing.this, BillingInfo.class);
            startActivity(i);
        }

    }
}
