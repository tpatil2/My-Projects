package android_project.com.doctorassistant;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

/**
 * Created by RAHUL on 4/26/2015.
 */
public class AddPatient extends Activity implements View.OnClickListener{
    databasehelper helper;
    Button b;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.add_patient);
        b = (Button)findViewById(R.id.Baddpatient);
        b.setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        if(v.getId() ==R.id.Baddpatient)
        {
            Toast temp;
            temp = Toast.makeText(AddPatient.this, "Patient Information Successfully Added.", Toast.LENGTH_SHORT);
            temp.show();
            EditText pname = (EditText) findViewById(R.id.nameetxt);
            EditText paddress = (EditText) findViewById(R.id.addressetxt);
            EditText pcontact = (EditText) findViewById(R.id.contactetxt);
            EditText pemail = (EditText) findViewById(R.id.emailetxt);
            EditText pgender = (EditText) findViewById(R.id.genderetxt);
            EditText pdob = (EditText) findViewById(R.id.dobetxt);
            EditText pnotes = (EditText) findViewById(R.id.notesetxt);

            String namestr = pname.getText().toString();
            String addstr = paddress.getText().toString();
            String contactstr = pcontact.getText().toString();
            String emailstr = pemail.getText().toString();
            String genderstr = pgender.getText().toString();
            String dobstr = pdob.getText().toString();
            String notesstr = pnotes.getText().toString();
            //Toast.makeText(AddPatient.this, namestr+addstr+contactstr+dobstr+genderstr+emailstr+notesstr,Toast.LENGTH_SHORT).show();
            helper = new databasehelper(namestr,addstr,contactstr,emailstr, genderstr,dobstr, notesstr);
            helper.insert_patient();
            Intent i = new Intent(getApplicationContext(), homepage.class);
            startActivity(i);

        }
    }
}
