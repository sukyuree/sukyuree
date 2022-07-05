package com.example.restaurantreserv;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.content.DialogInterface;
import android.icu.util.Calendar;
import android.os.Bundle;
import android.view.MenuInflater;
import android.view.View;
import android.widget.CalendarView;
import android.widget.EditText;
import android.widget.TimePicker;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.Button;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        CalendarView cal = (CalendarView)findViewById(R.id.calendarView);
        cal.setOnDateChangeListener(new CalendarView.OnDateChangeListener() {
            @Override
            public void onSelectedDayChange(@NonNull CalendarView view, int year, int month, int dayOfMonth) {
                EditText date = (EditText)findViewById(R.id.textDate);
                date.setText(year+"/"+(month+1)+"/"+dayOfMonth);
            }
        });
        TimePicker timePicker = (TimePicker)findViewById(R.id.timepicker);
        timePicker.setOnTimeChangedListener(new TimePicker.OnTimeChangedListener() {
            @Override
            public void onTimeChanged(TimePicker view, int hourOfDay, int minute) {
                EditText time = (EditText)findViewById(R.id.textTime);
                time.setText(hourOfDay+":"+minute);
            }
        });
        Button reserve_button =findViewById(R.id.button);
        reserve_button.setOnClickListener(new View.OnClickListener(){
            public void onClick(View view){
                show_reserve();
            }
        });
    }
    public boolean onCreateOptionsMenu(Menu menu){
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.reservmenu,menu);
        return true;
    }

    public boolean onOptionsItemSelected(MenuItem item){
        if(item.getItemId()==R.id.clear){
            final Calendar c= Calendar.getInstance();
            CalendarView cal = (CalendarView)findViewById(R.id.calendarView);
            TimePicker timePicker = (TimePicker)findViewById(R.id.timepicker);
            cal.setDate(c.getTimeInMillis());
            int hour = c.get(Calendar.HOUR_OF_DAY);
            int min = c.get(Calendar.MINUTE);
            int current_mon = c.get(Calendar.MONTH);
            int current_date = c.get(Calendar.DATE);
            int current_year = c.get(Calendar.YEAR);
            timePicker.setHour(hour);
            timePicker.setMinute(min);
            EditText date = (EditText)findViewById(R.id.textDate);
            date.setText(current_year+"/"+(current_mon+1)+"/"+current_date);
        }
        return true;
    }
    void show_reserve(){
        AlertDialog.Builder bd = new AlertDialog.Builder (MainActivity.this);
        EditText date = (EditText)findViewById(R.id.textDate);
        EditText time = (EditText)findViewById(R.id.textTime);
        bd.setTitle("RESERVE");
        bd.setMessage("DATE : "+date.getText()+", TIME : "+time.getText());
        bd.setPositiveButton("Yes", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                Toast.makeText(getBaseContext(),"CONFIRMED", Toast.LENGTH_SHORT).show();
            }
        });
        bd.setNegativeButton("No", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                Toast.makeText(getBaseContext(),"CANCLED",Toast.LENGTH_SHORT).show();
            }
        });
        bd.show();
    }
}
