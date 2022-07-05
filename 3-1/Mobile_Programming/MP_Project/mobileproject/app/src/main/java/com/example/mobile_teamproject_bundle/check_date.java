package com.example.mobile_teamproject_bundle;

import androidx.appcompat.app.AppCompatActivity;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

public class check_date extends AppCompatActivity {
    String FILENAME = "check_date.txt";
    int count=0;

    public void check_date_Write(String string){
        FileWriter fw = null ;
        File file = new File("/data/data/com.example.mobile_teamproject_bundle/files/");
        if(!file.exists()){ // 폴더 없을 경우
            file.mkdir(); // 폴더 생성
        }
        try{
            fw = new FileWriter(file+"/"+FILENAME);
            fw.write(string);
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

    public void check_date_Read(String check){
        File file = new File("/data/data/com.example.mobile_teamproject_bundle/files/");

        try{
            BufferedReader in = new BufferedReader(new FileReader(file+"/"+FILENAME));
            String line;
            while((line=in.readLine())!=null){
                check = line;
            }
            in.close();
        }
        catch(IOException e){

        }
    }
}

