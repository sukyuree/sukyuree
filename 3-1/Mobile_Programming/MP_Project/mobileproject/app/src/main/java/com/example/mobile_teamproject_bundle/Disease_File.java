package com.example.mobile_teamproject_bundle;

import androidx.appcompat.app.AppCompatActivity;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;

public class Disease_File extends AppCompatActivity {
    String FILENAME = "disease_list.txt";
    public void Disease_Write(){
        FileWriter fw = null ;
        File file = new File("/data/data/com.example.mobile_teamproject_bundle/files/");
        if(!file.exists()){ // 폴더 없을 경우
            file.mkdir(); // 폴더 생성
        }
        try{
            fw = new FileWriter(file+"/"+FILENAME);
            fw.write(
                    "암,달걀_오트밀_아보카도_두부_콩\n" +
                    "심장,연어_콩_감자_당근_브로콜리\n" +
                    "폐렴,무_배_도라지_갓김치\n" +
                    "당뇨,당근_블루베리_아스파라거스_양파\n" +
                    "고혈압,브로콜리_비트_허브\n" +
                    "저혈압,버터_시금치_우유_콩_치즈\n" +
                    "뇌혈관,당근_마늘_고등어\n" +
                    "치매,아보카도_강황_브로콜리_올리브유\n" +
                    "간질환,부추_홍합_토마토_조개_브로콜리_버섯"
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

    public void Disease_Read(ArrayList<Disease> ds){
        File file = new File("/data/data/com.example.mobile_teamproject_bundle/files/");
        try{
            BufferedReader in = new BufferedReader(new FileReader(file+"/"+FILENAME));
            String line;

            while((line=in.readLine())!=null){
                Disease dis_list = new Disease();
                String[] split = line.split(",");
                dis_list.disease_name=split[0];
                dis_list.disease_food=split[1];
                String[] split2 = split[1].split("_");
//                int i = 0;
//                while(split2[i]!=null) {
//                    dis_list.disease_foods.add(split2[i]);
//                    i++;
//                }
                for(String s : split2) {
                    dis_list.disease_foods.add(s);
                }
                ds.add(dis_list);
            }
            in.close();
        }
        catch(IOException e){

        }
    }
}
