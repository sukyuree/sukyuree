package com.example.mobile_teamproject_bundle;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;
import com.example.mobile_teamproject_bundle.*;
import androidx.appcompat.app.AppCompatActivity;

import java.util.ArrayList;

public class InsertActivity extends AppCompatActivity {
    //    public DbOpenHelper user = new DbOpenHelper();
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.startmenu);
        Button finish = findViewById(R.id.finish);
        finish.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                User user = new User();
                User_File file = new User_File();
                EditText name = (EditText) findViewById(R.id.ent_name);
                user.name = String.valueOf(name.getText());
                EditText age = (EditText) findViewById(R.id.ent_age);
                user.age = String.valueOf(age.getText());
                EditText height = (EditText) findViewById(R.id.ent_height);
                user.height = String.valueOf(height.getText());
                EditText weight = (EditText) findViewById(R.id.ent_weight);
                user.weight = String.valueOf(weight.getText());
                CheckBox check1 = (CheckBox) findViewById(R.id.checkBox1);
                CheckBox check2 = (CheckBox) findViewById(R.id.checkBox2);
                CheckBox check3 = (CheckBox) findViewById(R.id.checkBox3);
                CheckBox check4 = (CheckBox) findViewById(R.id.checkBox4);
                CheckBox check5 = (CheckBox) findViewById(R.id.checkBox5);
                CheckBox check6 = (CheckBox) findViewById(R.id.checkBox6);
                CheckBox check7 = (CheckBox) findViewById(R.id.checkBox7);
                CheckBox check8 = (CheckBox) findViewById(R.id.checkBox8);
                CheckBox check9 = (CheckBox) findViewById(R.id.checkBox9);
                user.disease="";
                if(check1.isChecked()==true) user.disease=String.valueOf(check1.getText())+"_";
                if(check2.isChecked()==true) user.disease=user.disease+String.valueOf(check2.getText())+"_";
                if(check3.isChecked()==true) user.disease=user.disease+String.valueOf(check3.getText())+"_";
                if(check4.isChecked()==true) user.disease=user.disease+String.valueOf(check4.getText())+"_";
                if(check5.isChecked()==true) user.disease=user.disease+String.valueOf(check5.getText())+"_";
                if(check6.isChecked()==true) user.disease=user.disease+String.valueOf(check6.getText())+"_";
                if(check7.isChecked()==true) user.disease=user.disease+String.valueOf(check7.getText())+"_";
                if(check8.isChecked()==true) user.disease=user.disease+String.valueOf(check8.getText())+"_";
                if(check9.isChecked()==true) user.disease=user.disease+String.valueOf(check9.getText())+"_";
                user.body_fat_rate=user.getBody_Fat_Rate(user);
                System.out.println(user.body_fat_rate);
                user.body = user.getBody_Form(Float.parseFloat(user.body_fat_rate));
                String user_information = user.name+","+user.age+","+user.height+","+user.weight+","
                        +user.body_fat_rate+","+user.body+","+user.disease;
                file.User_Write(user_information);
                System.out.println(user_information);
                Body_File create_body_file = new Body_File();
                create_body_file.Body_Write();
                Disease_File create_disease_file = new Disease_File();
                create_disease_file.Disease_Write();
                Exercise_File write_exercise = new Exercise_File();
                write_exercise.Exercise_Write();
                check_date cd = new check_date();
                cd.check_date_Write("FALSE");
                make_week_exercise();
//                String selected_exercise;
//                String exercise1 = new String();
//                String exercise2 = new String();
//                String exercise3 = new String();
//                ArrayList<Exercise> exercise_list = new ArrayList<Exercise>();
//                Exercise_File read_exercise = new Exercise_File();
//                read_exercise.Exercise_Read(exercise_list);
//                select_exercise(user,exercise_list,exercise1,exercise2,exercise3);
//                selected_exercise="월,"+exercise1+"_"+exercise2+"_"+exercise3;
//                select_exercise(user,exercise_list,exercise1,exercise2,exercise3);
//                selected_exercise=selected_exercise+"\n"+"화,"+exercise1+"_"+exercise2+"_"+exercise3;
//                select_exercise(user,exercise_list,exercise1,exercise2,exercise3);
//                selected_exercise=selected_exercise+"\n"+"수,"+exercise1+"_"+exercise2+"_"+exercise3;
//                select_exercise(user,exercise_list,exercise1,exercise2,exercise3);
//                selected_exercise=selected_exercise+"\n"+"목,"+exercise1+"_"+exercise2+"_"+exercise3;
//                select_exercise(user,exercise_list,exercise1,exercise2,exercise3);
//                selected_exercise=selected_exercise+"\n"+"금,"+exercise1+"_"+exercise2+"_"+exercise3;
//                select_exercise(user,exercise_list,exercise1,exercise2,exercise3);
//                selected_exercise=selected_exercise+"\n"+"토,"+exercise1+"_"+exercise2+"_"+exercise3;
//                select_exercise(user,exercise_list,exercise1,exercise2,exercise3);
//                selected_exercise=selected_exercise+"\n"+"일,"+exercise1+"_"+exercise2+"_"+exercise3;
//                Date_Exercise write_user_exercise = new Date_Exercise();
//                write_user_exercise.Date_Exercise_Write(selected_exercise);


                setContentView(R.layout.waiting);

                startService(new Intent(getApplicationContext(), SelectDiet.class));
            }
        });

    }


    public String select_exercise(User user, ArrayList<Exercise> exercise_list) {
        int randomValue = 0;
        String exercise1 = "";
        String exercise2 = "";
        String exercise3 = "";
        randomValue = (int) (Math.random() * (user.diseases.size() - 1));
        for (int i = 0; i < exercise_list.size(); i++) {
            if (user.diseases.get(randomValue).equals(exercise_list.get(i).disease_name)) {
                int a[] = new int[3];
                for (int k = 0; k < 3; k++) {
                    a[k] = (int) (Math.random() * (exercise_list.get(i).exercise_names.size() - 1));
                    for (int j = 0; j < k; j++) {
                        if (a[k] == a[j]) {
                            k--;
                            break;
                        }
                    }
                }
                exercise1 = exercise_list.get(i).exercise_names.get(a[0]);
                exercise2 = exercise_list.get(i).exercise_names.get(a[1]);
                exercise3 = exercise_list.get(i).exercise_names.get(a[2]);

            }
        }
        return exercise1+"_"+exercise2+"_"+exercise3;
    }

    public void make_week_exercise(){
        User user = new User();
        User_File uf = new User_File();
        uf.User_Read(user);
        String selected_exercise;
        String k;

        ArrayList<Exercise> exercise_list = new ArrayList<Exercise>();

        Exercise_File read_exercise = new Exercise_File();
        read_exercise.Exercise_Read(exercise_list);
        k=select_exercise(user,exercise_list);
        selected_exercise="Mon,"+k+"\n";
        k=select_exercise(user,exercise_list);
        selected_exercise=selected_exercise+"Tue,"+k+"\n";
        k=select_exercise(user,exercise_list);
        selected_exercise=selected_exercise+"Wed,"+k+"\n";
        k=select_exercise(user,exercise_list);
        selected_exercise=selected_exercise+"Thu,"+k+"\n";
        k=select_exercise(user,exercise_list);
        selected_exercise=selected_exercise+"Fri,"+k+"\n";
        k=select_exercise(user,exercise_list);
        selected_exercise=selected_exercise+"Sat,"+k+"\n";
        k=select_exercise(user,exercise_list);
        selected_exercise=selected_exercise+"Sun,"+k;

        Date_Exercise write_user_exercise = new Date_Exercise();
        write_user_exercise.Date_Exercise_Write(selected_exercise);
    }
}
