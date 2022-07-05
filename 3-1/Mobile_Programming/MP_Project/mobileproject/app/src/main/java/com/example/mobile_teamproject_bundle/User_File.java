package com.example.mobile_teamproject_bundle;

import androidx.appcompat.app.AppCompatActivity;
import java.io.FileInputStream;
import java.io.*;
import android.os.Environment;
public class User_File extends AppCompatActivity {
    String FILENAME = "user.txt";

    public void User_Write(String user_information){
        FileWriter fw = null ;
        File file = new File("/data/data/com.example.mobile_teamproject_bundle/files/");
        if(!file.exists()){ // 폴더 없을 경우
            file.mkdir(); // 폴더 생성
        }
        try{
//            BufferedWriter writer = new BufferedWriter(new FileWriter(file+FILENAME, true));
//            writer.append(user_information);
//            writer.flush();
//            writer.close();
              fw = new FileWriter(file+"/"+FILENAME);
              fw.write(user_information);
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

    public void User_Read(User user){
        File file = new File("/data/data/com.example.mobile_teamproject_bundle/files/");
        FileReader fr = null;
        if(!file.exists()){ // 폴더 없을 경우
            file.mkdir(); // 폴더 생성
        }
        try{
            BufferedReader in = new BufferedReader(new FileReader(file+"/"+FILENAME));
            String line;
            while((line=in.readLine())!=null){
                String[] split = line.split(",");
                user.name=split[0];
                user.age=split[1];
                user.height=split[2];
                user.weight=split[3];
                user.body_fat_rate=split[4];
                user.body=split[5];
                user.disease=split[6];
                String[] split2 = split[6].split("_");
                for(String s : split2) {
                    user.diseases.add(s);
                }
            }
//            fr = new FileReader((file+"/"+FILENAME));
//            while((data = fr.read())!=-1){
//                ch = (char)data;
//                System.out.println(ch);
//            }
            in.close();
        }
        catch(IOException e) {
            System.out.println("read err\n");
            e.printStackTrace();
        }
    }


}
