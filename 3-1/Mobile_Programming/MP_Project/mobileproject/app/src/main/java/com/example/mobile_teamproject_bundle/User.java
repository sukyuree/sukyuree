package com.example.mobile_teamproject_bundle;

import java.util.ArrayList;

public class User {
    String name;
    String age;
    String height;
    String weight;
    String disease;
    String body_fat_rate;
    String body;
    ArrayList<String> diseases = new ArrayList<String>();

    public String getBody_Fat_Rate(User user){
        double height=(Double.parseDouble(user.height)/100);
        double weight=Double.parseDouble(user.weight);;
        double BMI;
        BMI=weight/(height * height);
        return ""+String.format("%.1f",BMI);
    }

    public String getBody_Form(float BMI){
        if(BMI<(185/100)) return "저체중";
        else if(BMI<23 && BMI>=(185/100)) return "정상체중";
        else if(BMI>=23 && BMI < 25) return "과제충";
        else return "비만";
    }
}
