package android_project.com.doctorassistant;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

/**
 * Created by Tejas on 4/26/15.
 */
public class Mainpage_appointment extends Activity implements View.OnClickListener {

    Button a1, a2, a3;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.mainpage_appointment);

        a1 = (Button)findViewById(R.id.b_make_appoint);
        a1.setOnClickListener(this);

        a2 = (Button)findViewById(R.id.b_edit_appoint);
        a2.setOnClickListener(this);

        a3 = (Button)findViewById(R.id.b_view_appoint);
        a3.setOnClickListener(this);

    }

    @Override
    public void onClick(View v) {
        if(v.getId()==R.id.b_make_appoint)
        {
            Intent i = new Intent(Mainpage_appointment.this, Makeappointment.class);
            startActivity(i);
        }

        if(v.getId()==R.id.b_edit_appoint)
        {
            Intent i = new Intent(Mainpage_appointment.this, EditAppointment.class);
            startActivity(i);
        }

        if(v.getId()==R.id.b_view_appoint)
        {
            Intent i = new Intent(Mainpage_appointment.this, ViewAppointment.class);
            startActivity(i);
        }

    }
}
