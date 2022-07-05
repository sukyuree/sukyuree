package com.example.mobile_teamproject_bundle;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;

public class Exercise_File {
    String FILENAME = "exercise_list.txt";
    public void Exercise_Write(){
        FileWriter fw = null ;
        File file = new File("/data/data/com.example.mobile_teamproject_bundle/files/");
        if(!file.exists()){ // 폴더 없을 경우
            file.mkdir(); // 폴더 생성
        }
        try{
            fw = new FileWriter(file+"/"+FILENAME);
            fw.write(
                    "암,요가_스쿼트_런지_윈드밀 런지_플랭크 잭_워킹 햄스트링칭_유산소 운동\n" +
                            "심장,걷기_자전거 타기_스쿼트_플랭크_런지_케틀벨_독수리 운동_심장 스트레칭\n" +
                            "폐렴,수영_사이클 운동_상체 근력 운동_복식호흡 운동_호흡 재활 치료\n" +
                            "당뇨,산책_조깅_맨손체조_윈드밀 런지_종아리 운동_허벅지 운동_전신 유산소운동_하체운동\n" +
                            "고혈압,가슴 등척성 운동_복부 등척성 운동_허벅지 등척성 운동\n" +
                            "저혈압,러닝_수영_하체 근력 운동\n" +
                            "뇌혈관,러닝_조깅_자전거 타기_수영\n" +
                            "치매,치매 예방 운동_치매 예방 게임_치매 예방 체조_치매 예방 박수\n" +
                            "간,조깅_수영_자전거 타기_간 스트레칭_간 독소 제거 운동_간질환 운동_간 해독 운동"
            );
            //fw.close();
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

    public void Exercise_Read(ArrayList<Exercise> ex){
        File file = new File("/data/data/com.example.mobile_teamproject_bundle/files/");
        try{
            BufferedReader in = new BufferedReader(new FileReader(file+"/"+FILENAME));
            String line;

            while((line=in.readLine())!=null){
                Exercise ex_list = new Exercise();
                String[] split = line.split(",");
                ex_list.disease_name=split[0];
                ex_list.exercise_name=split[1];
                String[] split2 = split[1].split("_");
//                int i = 0;
//                while(split2[i]!=null) {
//                    dis_list.disease_foods.add(split2[i]);
//                    i++;
//                }
                for(String s : split2) {
                    ex_list.exercise_names.add(s);
                }
                ex.add(ex_list);
            }
            in.close();
        }
        catch(IOException e){

        }
    }
}
