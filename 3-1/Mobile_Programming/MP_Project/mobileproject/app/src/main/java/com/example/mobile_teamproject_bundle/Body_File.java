package com.example.mobile_teamproject_bundle;

import androidx.appcompat.app.AppCompatActivity;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
public class Body_File extends AppCompatActivity {
    String FILENAME = "body_list.txt";
    int count=0;

    public void Body_Write(){
        FileWriter fw = null ;
        File file = new File("/data/data/com.example.mobile_teamproject_bundle/files/");
        if(!file.exists()){ // 폴더 없을 경우
            file.mkdir(); // 폴더 생성
        }
        try{
            fw = new FileWriter(file+"/"+FILENAME);
            fw.write("저체중,치즈_우유_호두_콩_달걀_두부\n" +
                    "과체중,갈치_고등어_해산물_닭고기_미역_김\n" +
                    "비만,고구마_닭가슴살_두부"
            );
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

    public void Body_Read(ArrayList<Body> bd){
        File file = new File("/data/data/com.example.mobile_teamproject_bundle/files/");

        try{
            BufferedReader in = new BufferedReader(new FileReader(file+"/"+FILENAME));
            String line;
            while((line=in.readLine())!=null){
                //System.out.println(line);
                Body body_list = new Body();
                String[] split = line.split(",");
                body_list.body_name=split[0];
                body_list.body_food=split[1];
                System.out.println(body_list.body_food);
                String[] split2 = body_list.body_food.split("_");
//                int i =0;
//                while(split2[i]!=null){
//                    body_list.body_foods.add(split2[i]);
//                    i++;
//                }
                for(String s : split2) {
                    body_list.body_foods.add(s);
                }
                bd.add(body_list);
            }
            in.close();
        }
        catch(IOException e){

        }
    }
}
