package com.example.mobile_teamproject_bundle;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.NotificationCompat;
import android.app.Activity;
import android.app.AlarmManager;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.net.Uri;
import android.os.Bundle;
import android.provider.MediaStore;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.TextView;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.Locale;
import java.util.Set;

public class MainActivity extends AppCompatActivity{
    private static final int NOTIFICATION_ID = 12;
    public static final String PREFS_NAME = "MyPref";
    private final int GET_GALLERY = 20;

    String foodNm;
    String recipeOrder;
    String ingredients;
    String foodimg;
    String category;

    String exercise1;
    String exercise2;
    String exercise3;
    String x_Day;
    boolean New_recommend;
    int first = 0;
    ImageView profile_picture;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.mainmenu);

        SharedPreferences settings = getSharedPreferences(PREFS_NAME, 0);
        x_Day = settings.getString("xDay","Tue");

        TextView main_name = findViewById(R.id.ented_name);
        TextView main_age = findViewById(R.id.ented_age);
        TextView main_height = findViewById(R.id.ented_height);
        TextView main_weight = findViewById(R.id.ented_weight);
        TextView main_disease = findViewById(R.id.ented_disease);
        TextView main_BMI = findViewById(R.id.ented_bfr);
        TextView main_body = findViewById(R.id.che);
        profile_picture = findViewById(R.id.imageView3);
        User user = new User();
        User_File file = new User_File();
        file.User_Read(user);
        main_name.setText(user.name);
        main_age.setText(user.age);
        main_height.setText(user.height);
        main_weight.setText(user.weight);
        main_body.setText(user.body);
        main_BMI.setText(user.body_fat_rate);
        main_disease.setText(user.disease);
        profile_picture.setImageResource(R.drawable.logo);
        profile_picture.setOnClickListener(new View.OnClickListener(){
            public void onClick(View view){
                Intent intent = new Intent(Intent.ACTION_PICK);
                intent.setDataAndType(MediaStore.Images.Media.EXTERNAL_CONTENT_URI,"image/*");
                startActivityForResult(intent,GET_GALLERY);
            }
        });
        TextView food_name = findViewById(R.id.food_name);
        ImageView food_img = findViewById(R.id.food_img);
        TextView food_info = findViewById(R.id.food_script);

        Body_File read_body = new Body_File();
        Disease_File read_disease = new Disease_File();
        ArrayList<Body> body = new ArrayList<Body>();
        ArrayList<Disease> disease = new ArrayList<Disease>();
        read_body.Body_Read(body);
        read_disease.Disease_Read(disease);

        System.out.println(disease.get(0).disease_name);
        System.out.println(disease.get(0).disease_foods.get(0));
        System.out.println(disease.get(0).disease_foods.get(1));

        Intent intent = getIntent();
        foodNm = intent.getStringExtra("foodName");
        recipeOrder = intent.getStringExtra("recipeOrder");
        ingredients = intent.getStringExtra("materialInfo");
        foodimg = intent.getStringExtra("foodImage");
        category = intent.getStringExtra("calorieInfo");

        food_name.setText(intent.getStringExtra("foodName"));
        if(foodimg==null) {
            food_img.setImageResource(R.drawable.noimg);
        }
        else GlideApp.with(this).load(foodimg).into(food_img);
        food_info.setText(intent.getStringExtra("recipeOrder"));

        AlarmManager alarmManager = (AlarmManager) this.getSystemService(Context.ALARM_SERVICE);
        Intent receiverIntent = new Intent(MainActivity.this, AlarmRecevier.class);
        PendingIntent pIntent = PendingIntent.getBroadcast(this, 0, receiverIntent, 0);
        //alarmManager.cancel(pIntent);
        if (alarmManager!= null) {
            Log.d("alarm","12");
            alarmManager.cancel(pIntent);
        }
        Calendar calendar = Calendar.getInstance();
        calendar.setTimeInMillis(System.currentTimeMillis());
        calendar.set(Calendar.HOUR_OF_DAY, 12);
        calendar.set(Calendar.MINUTE,0);
        calendar.set(Calendar.SECOND,0);

        if(Calendar.getInstance().after(calendar)){
            calendar.add(Calendar.DAY_OF_MONTH,1);
        }

        //alarmManager.setExact(AlarmManager.RTC_WAKEUP, calendar.getTimeInMillis(), pIntent);
        //Log.d("day", String.valueOf(calendar.DAY_OF_MONTH));
       // Log.d("hour", String.valueOf(calendar.HOUR_OF_DAY));
        Log.d("calender", String.valueOf(calendar.getTime()));
        alarmManager.set(AlarmManager.RTC_WAKEUP, calendar.getTimeInMillis(),pIntent);//InexactRepeating(AlarmManager.RTC_WAKEUP, calendar.getTimeInMillis(), AlarmManager.INTERVAL_DAY, pIntent);

        AlarmManager alarm= (AlarmManager) this.getSystemService(Context.ALARM_SERVICE);
        Intent receiver = new Intent(MainActivity.this, AlarmD.class);
        PendingIntent pIn = PendingIntent.getBroadcast(this, 1, receiver, 0);
        //alarmManager.cancel(pIn);
        if (alarm!= null) {
            Log.d("alarm","19");
            alarm.cancel(pIntent);
        }
        Calendar cal = Calendar.getInstance();
        cal.setTimeInMillis(System.currentTimeMillis());
        cal.set(cal.HOUR_OF_DAY,10);
        cal.set(cal.MINUTE,0);
        calendar.set(Calendar.SECOND,0);

        if(Calendar.getInstance().after(cal)){
            cal.add(Calendar.DAY_OF_MONTH,1);
        }
        //Log.d("day", String.valueOf(cal.DAY_OF_MONTH));
        //Log.d("hour", String.valueOf(cal.HOUR_OF_DAY));
        Log.d("calender dinner", String.valueOf(cal.getTime()));
        alarm.setExact(AlarmManager.RTC_WAKEUP, cal.getTimeInMillis(),pIn);// + AlarmManager.INTERVAL_DAY, pIn);

        Date currentTime = Calendar.getInstance().getTime();
        SimpleDateFormat weekdayFormat = new SimpleDateFormat("EE", Locale.getDefault());
        String weekDay=weekdayFormat.format(currentTime);


        ArrayList<Week_Date> week_exercise = new ArrayList<>();
        check_date check = new check_date();
        Week_Date today = new Week_Date();
        String new_one = new String();
        check.check_date_Read(new_one);
        ListView ex_view = (ListView) findViewById(R.id.exercise_listview);
        Date_Exercise read_week_exercise = new Date_Exercise();
        read_week_exercise.Date_Exercise_Read(week_exercise);
        ArrayList<String> exercise_list = new ArrayList<String>();

        if(weekDay.equals("Mon")&&new_one.equals("FALSE")){
            InsertActivity make_file = new InsertActivity();
            make_file.make_week_exercise();
            exercise_list.add(week_exercise.get(0).EXERCISE1);
            exercise_list.add(week_exercise.get(0).EXERCISE2);
            exercise_list.add(week_exercise.get(0).EXERCISE3);
            check.check_date_Write("TRUE");
        }
        else if(weekDay.equals("Mon")&&new_one.equals("TRUE")){
            exercise_list.add(week_exercise.get(0).EXERCISE1);
            exercise_list.add(week_exercise.get(0).EXERCISE2);
            exercise_list.add(week_exercise.get(0).EXERCISE3);
        }
        else {
            for(int i=0;i<7;i++){
                if(weekDay.equals(week_exercise.get(i).DAY)){
                    //System.out.println("HI");
                    check.check_date_Write("FALSE");
                    exercise_list.add(week_exercise.get(i).EXERCISE1);
                    exercise_list.add(week_exercise.get(i).EXERCISE2);
                    exercise_list.add(week_exercise.get(i).EXERCISE3);
                }
            }
        }

        Log.d("exercise_list", String.valueOf(exercise_list));

        ArrayAdapter ad = new ArrayAdapter(this,android.R.layout.simple_list_item_1,exercise_list);
        ex_view.setAdapter(ad);
        ex_view.setOnItemClickListener(new AdapterView.OnItemClickListener(){
            @Override
            public void onItemClick(AdapterView parent, View v, int position, long id){
                Intent intent = new Intent(getApplicationContext(), ExercisingActivity.class);
                //boolean back = intent.getBooleanExtra("back",false);
                //Log.d("back", String.valueOf(back));
                Log.d("x_day",x_Day);
                Log.d("today",weekDay);
                //Log.d("equal?", String.valueOf(x_Day.equals(weekDay)));
                if(first==0 &&(!x_Day.equals(weekDay))) {
                    Log.d("true","haha");
                    New_recommend = true;  //0
                    first++;
                }
                else if(first!=0){
                    Log.d("flase","haha");
                    New_recommend = false; //rating복구
                }
                intent.putExtra("운동",ad.getItem(position).toString());
                String[] array = exercise_list.toArray(new String[exercise_list.size()]);
                intent.putExtra("recommended",array);
                intent.putExtra("IsNew",New_recommend);
                startActivity(intent);//startActivity(intent);

                SharedPreferences setting = getSharedPreferences(PREFS_NAME, 0);
                SharedPreferences.Editor editor = setting.edit();
                editor.putString("xDay",weekDay);
            }
        });
    }

    public void toRecipe(View target) {
        Intent intent = new Intent(getApplicationContext(), SubActivity.class);
        intent.putExtra("name",foodNm);
        intent.putExtra("recipe",recipeOrder);
        intent.putExtra("ingredients",ingredients);
        intent.putExtra("image",foodimg);
        intent.putExtra("info",category);
        startActivity(intent);
    }
    public void toWeek(View target) {
        Intent intent = new Intent(getApplicationContext(), ExercisePlanActivity.class);
        startActivity(intent);
    }
    public void modifyUserInformation(View target) {
        Intent intent = new Intent(getApplicationContext(), InsertActivity.class);
        startActivity(intent);
    }
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == GET_GALLERY && resultCode == RESULT_OK && data != null && data.getData() != null) {
             Uri selected = data.getData();
             profile_picture.setImageURI(selected);
        }
    }
}
