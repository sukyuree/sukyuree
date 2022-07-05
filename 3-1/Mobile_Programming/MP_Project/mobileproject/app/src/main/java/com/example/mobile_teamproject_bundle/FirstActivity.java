package com.example.mobile_teamproject_bundle;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.example.mobile_teamproject_bundle.*;
import androidx.appcompat.app.AppCompatActivity;

public class FirstActivity extends AppCompatActivity {
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.first_activity);
    }

    public void toInsert(View target) {
        CheckInformation check = new CheckInformation();
        if (check.CheckExerciseInformation() ==false&&check.CheckUserInformation() == false) {
            Intent intent = new Intent(getApplicationContext(), InsertActivity.class);
            startActivity(intent);
        } else if (check.CheckExerciseInformation() ==true&&check.CheckUserInformation() == true) {
            setContentView(R.layout.waiting);
            User user = new User();
            User_File file = new User_File();
            file.User_Read(user);
            TextView forYou = (TextView)findViewById(R.id.ForU);
            forYou.setText(user.name+"님을 위한 식단과 운동을 추천 중입니다 :)");
            startService(new Intent(getApplicationContext(), SelectDiet.class));
        }
    }

    public void toExercisePlan(View target) {
        Intent intent = new Intent(getApplicationContext(), ExercisePlanActivity.class);
        startActivity(intent);
    }
}
