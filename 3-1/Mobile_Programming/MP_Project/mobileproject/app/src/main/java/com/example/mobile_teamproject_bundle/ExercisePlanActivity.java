package com.example.mobile_teamproject_bundle;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.util.Log;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.ProgressBar;

import androidx.appcompat.app.AppCompatActivity;

import java.util.ArrayList;
import java.util.Arrays;

public class ExercisePlanActivity  extends AppCompatActivity {
    public static final String PREFS_NAME = "MyPrfs";
    int[] rating = new int[3];
    int[] weeklyRating = new int[21];
    int rate;
    int result;
    ArrayList<Integer> week = new ArrayList<Integer>();
    boolean IsNew = true;
    ArrayList<String> week_exercises = new ArrayList<>();
    String[] exercises = {"운동","운동","운동"};

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.exercise_weekly);

        ArrayList<Week_Date> week_exercise = new ArrayList<>();
        Date_Exercise read_week_exercise = new Date_Exercise();
        read_week_exercise.Date_Exercise_Read(week_exercise);

        exercises[0]=week_exercise.get(0).EXERCISE1;
        exercises[1]=week_exercise.get(0).EXERCISE2;
        exercises[2]=week_exercise.get(0).EXERCISE3;
        week_exercises.add(Arrays.toString(exercises));
        exercises[0]=week_exercise.get(1).EXERCISE1;
        exercises[1]=week_exercise.get(1).EXERCISE2;
        exercises[2]=week_exercise.get(1).EXERCISE3;
        week_exercises.add(Arrays.toString(exercises));
        exercises[0]=week_exercise.get(2).EXERCISE1;
        exercises[1]=week_exercise.get(2).EXERCISE2;
        exercises[2]=week_exercise.get(2).EXERCISE3;
        week_exercises.add(Arrays.toString(exercises));
        exercises[0]=week_exercise.get(3).EXERCISE1;
        exercises[1]=week_exercise.get(3).EXERCISE2;
        exercises[2]=week_exercise.get(3).EXERCISE3;
        week_exercises.add(Arrays.toString(exercises));
        exercises[0]=week_exercise.get(4).EXERCISE1;
        exercises[1]=week_exercise.get(4).EXERCISE2;
        exercises[2]=week_exercise.get(4).EXERCISE3;
        week_exercises.add(Arrays.toString(exercises));
        exercises[0]=week_exercise.get(5).EXERCISE1;
        exercises[1]=week_exercise.get(5).EXERCISE2;
        exercises[2]=week_exercise.get(5).EXERCISE3;
        week_exercises.add(Arrays.toString(exercises));
        exercises[0]=week_exercise.get(6).EXERCISE1;
        exercises[1]=week_exercise.get(6).EXERCISE2;
        exercises[2]=week_exercise.get(6).EXERCISE3;
        week_exercises.add(Arrays.toString(exercises));

        ListView monday = (ListView) findViewById(R.id.mon);
        ListView t = (ListView) findViewById(R.id.Tues);
        ListView w = (ListView) findViewById(R.id.W);
        ListView thur= (ListView) findViewById(R.id.Thursday);
        ListView fri = (ListView) findViewById(R.id.Fri);
        ListView sat = (ListView) findViewById(R.id.Satur);
        ListView sun = (ListView) findViewById(R.id.Sun);

        for(int i=0;i<week_exercises.size();i++) {
            if(week_exercises.get(i).contains("[")) {
                //Log.d("enter",week_exercises.get(i));
                week_exercises.set(i,week_exercises.get(i).replaceAll("\\[",""));
            }
            //Log.d("next",week_exercises.get(i));
            if(week_exercises.get(i).contains("]")) {
                week_exercises.set(i,week_exercises.get(i).replaceAll("\\]",""));
            }
        }
        Log.d("week", String.valueOf(week_exercises));
        String[] strArr =week_exercises.get(0).split(",");
        for(int i=0;i<strArr.length;i++){
            Log.d(String.valueOf(i),strArr[i]);
        }
        ArrayAdapter ad = new ArrayAdapter(this, R.layout.simple_list,strArr);
        monday.setAdapter(ad);
        String[] str =week_exercises.get(1).split(",");
        ArrayAdapter at = new ArrayAdapter(this,R.layout.simple_list,str);
        t.setAdapter(at);
        String[] wen =week_exercises.get(2).split(",");
        ArrayAdapter aw = new ArrayAdapter(this,R.layout.simple_list,wen);
        w.setAdapter(aw);
        String[] T =week_exercises.get(3).split(",");
        ArrayAdapter att = new ArrayAdapter(this,R.layout.simple_list,T);
        thur.setAdapter(att);
        String[] Fri =week_exercises.get(4).split(",");
        ArrayAdapter af = new ArrayAdapter(this,R.layout.simple_list,Fri);
        fri.setAdapter(af);
        String[] satur =week_exercises.get(5).split(",");
        ArrayAdapter as = new ArrayAdapter(this,R.layout.simple_list,satur);
        sat.setAdapter(as);
        String[] sunn =week_exercises.get(6).split(",");
        ArrayAdapter asun = new ArrayAdapter(this,R.layout.simple_list,sunn);
        sun.setAdapter(asun);

    }
    public void onStop() {
        super.onStop();
    }
}
