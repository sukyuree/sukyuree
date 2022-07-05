package com.example.mobile_teamproject_bundle;

import androidx.appcompat.app.AppCompatActivity;

import java.io.File;


public class CheckInformation extends AppCompatActivity {

    public boolean CheckUserInformation () {
        File file = new File("/data/data/com.example.mobile_teamproject_bundle/files/user.txt");

        if (!file.exists()) {
            System.out.println("No Files");
            return false;
        } else {
            return true;
        }
    }
    public boolean CheckExerciseInformation () {
            File file = new File("/data/data/com.example.mobile_teamproject_bundle/files/exercise_list.txt");

            if (!file.exists()) {
                System.out.println("No Files");
                return false;
            } else {
                return true;
            }
        }
    }
