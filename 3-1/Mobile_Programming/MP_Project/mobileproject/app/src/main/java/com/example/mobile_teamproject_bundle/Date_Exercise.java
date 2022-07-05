package com.example.mobile_teamproject_bundle;

import androidx.appcompat.app.AppCompatActivity;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;

public class Date_Exercise extends AppCompatActivity {
    String FILENAME = "week_exercise.txt";
    int count=0;

    public void Date_Exercise_Write(String Exercise){
        FileWriter fw = null ;
        File file = new File("/data/data/com.example.mobile_teamproject_bundle/files/");
        if(!file.exists()){ // 폴더 없을 경우
            file.mkdir(); // 폴더 생성
        }
        try{
            fw = new FileWriter(file+"/"+FILENAME);
            fw.write(Exercise);
            fw.close();
        }
        catch(IOException e){
            System.out.println("write err");
            e.printStackTrace();
        }

        if (fw!=null){
            try{
                fw.close();
            }
            catch( Exception e){
                e.printStackTrace();
            }
        }
    }

    public void Date_Exercise_Read(ArrayList<Week_Date> weekDate){
        File file = new File("/data/data/com.example.mobile_teamproject_bundle/files/");

        try{
            BufferedReader in = new BufferedReader(new FileReader(file+"/"+FILENAME));
            String line;

            int i=0;
            while((line=in.readLine())!=null){
                Week_Date wd= new Week_Date();
                String[] split = line.split(",");
                wd.DAY=split[0];
                String temp=split[1];
                String[] split2 = temp.split("_");
                wd.EXERCISE1=split2[0];
                wd.EXERCISE2=split2[1];
                wd.EXERCISE3=split2[2];
                weekDate.add(wd);
                i++;
            }
            in.close();
//            for(int k=0;k<i;k++){
//                System.out.println(weekDate.get(k).DAY);
//                System.out.println(weekDate.get(k).EXERCISE1);
//                System.out.println(weekDate.get(k).EXERCISE2);
//                System.out.println(weekDate.get(k).EXERCISE3);
//            }
        }
        catch(IOException e){

        }
    }
}
