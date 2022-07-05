package com.example.mobile_teamproject_bundle;

import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;

import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserFactory;

import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.URL;
import java.util.ArrayList;

public class RecommendCustomDietService extends Service {
    private Context mContext;
    private String cntntsNo;

    String foodNm;
    String foodImg;
    String foodInfo;
    String body_check = "정상체중";
    String[] mat;
    User user;

    private ArrayList<String> foodName;
    private ArrayList<String> foodImage;
    private ArrayList<String> materialInfo;
    private ArrayList<String> recipeOrder;
    private ArrayList<String> calorieInfo;
    private ArrayList<String> carbohydratesInfo;
    private ArrayList<String> proteinInfo;
    private ArrayList<String> lipidInfo;
    public ArrayList<Body> body = new ArrayList<Body>();
    public ArrayList<Disease> disease = new ArrayList<Disease>();
    private ArrayList<String> Dingredient= new ArrayList<>();

    FoodInfo_Parser Parser = new FoodInfo_Parser();
    ArrayList<PriceDataStruct> Data = new ArrayList<PriceDataStruct>();
    RecommendDietDetail xmlData = new RecommendDietDetail();

    public RecommendCustomDietService() {
    }

    @Override
    public IBinder onBind(Intent intent) {
        // TODO: Return the communication channel to the service.
        throw new UnsupportedOperationException("Not yet implemented");
    }

    public int onStartCommand(Intent intent, int flags, int startId) {
        mContext = this;

        user = new User();
        User_File file = new User_File();
        file.User_Read(user);

        Body_File read_body = new Body_File();
        Disease_File read_disease = new Disease_File();
        read_body.Body_Read(body);
        read_disease.Disease_Read(disease);

        new Thread(new Runnable() {
            @Override
            public void run() {
                Log.d("start","service");
                getAPI();
                Recommed();
            }
        }).start();
        return super.onStartCommand(intent, flags, startId);
    }
    private void getAPI() {

        String apiKey = "20210430IQ6XOLUFMGFHTTQEMNG1A";

        foodName = new ArrayList<>();
        foodImage = new ArrayList<>();
        materialInfo = new ArrayList<>();
        recipeOrder = new ArrayList<>();
        calorieInfo = new ArrayList<>();
        carbohydratesInfo = new ArrayList<>();
        proteinInfo = new ArrayList<>();
        lipidInfo = new ArrayList<>();

        cntntsNo = PreferenceManager.getString(mContext, "cntntsNo");
        String apiUrl = "http://api.nongsaro.go.kr/service/recomendDiet/recomendDietDtl?apiKey=" + apiKey
                + "&cntntsNo=" + cntntsNo;

        try {
            URL url = new URL(apiUrl);
            InputStream inputStream = url.openStream();

            XmlPullParserFactory factory = XmlPullParserFactory.newInstance();
            XmlPullParser xpp = factory.newPullParser();
            xpp.setInput(new InputStreamReader(inputStream, "UTF-8"));

            int eventType = xpp.getEventType();
            String tag = "";

            while (eventType != XmlPullParser.END_DOCUMENT) {
                tag = xpp.getName();
                switch (eventType) {
                    case XmlPullParser.START_TAG:
                        if (tag.equals("fdNm")) {
                            xpp.next();
                            foodName.add(xpp.getText());
                        } else if (tag.equals("rtnImageDc")) {
                            xpp.next();
                            foodImage.add(xpp.getText());
                        } else if (tag.equals("matrlInfo")) {
                            xpp.next();
                            //Log.d("mat",xpp.getText());
                            materialInfo.add(xpp.getText());
                            materialInfo.add("_");
                        } else if (tag.equals("ckngMthInfo")) {
                            xpp.next();
                            recipeOrder.add(xpp.getText());
                        } else if (tag.equals("clriInfo")) {
                            xpp.next();
                            calorieInfo.add(xpp.getText());
                            //materialInfo.add("_");
                        } else if (tag.equals("crbhInfo")) {
                            xpp.next();
                            carbohydratesInfo.add(xpp.getText());
                        } else if (tag.equals("protInfo")) {
                            xpp.next();
                            proteinInfo.add(xpp.getText());
                        } else if (tag.equals("ntrfsInfo")) {
                            xpp.next();
                            lipidInfo.add(xpp.getText());
                        }
                        break;
                    case XmlPullParser.END_TAG:
                        break;
                }
                eventType = xpp.next();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

    }
    private void Recommed () {
        //DB의 정보와 재료 비교하여 맞는 레시피 반환
        int num = 3;
        int position;
        Dingredient.clear();
        String[] sickness = user.disease.split("_");  //당뇨,암
        for (int i=0;i<9;i++) {
            for (int j =0; j < sickness.length; j++) {
                if (disease.get(i).disease_name.contains(sickness[j])) {
                    //Log.d("size", String.valueOf(disease.get(i).disease_foods.size()));
                    for(int k =0 ;k<disease.get(i).disease_foods.size();k++) {
                        //Log.d("add",disease.get(i).disease_foods.get(k));
                        Dingredient.add(disease.get(i).disease_foods.get(k));
                    }
                }
            }
        }
        //Log.d("질병 좋은 재료", String.valueOf(Dingredient));

        float body_fat =  Float.parseFloat(user.body_fat_rate);  //체지방률
        if(body_fat<18){
            body_check = "저체중";
        }else if(23<body_fat&&body_fat<25) body_check = "과체중";
        else if(25<body_fat) body_check = "비만";

        //Log.d("체질",body_check);

        for (int i=0;i<3;i++) {
            if (body.get(i).body_name.contains(body_check)) {
                for(int k =0 ;k<3;k++) {
                    Dingredient.add(body.get(i).body_foods.get(k));
                }
            }
        }
        Log.d("+체질 좋은 재료",String.valueOf(Dingredient));

        num = foodName.size();
        //position = (int) (Math.random() * (num - 1));
        //Log.d("size", String.valueOf(foodName.size()));
        //Log.d("foodname", String.valueOf(foodName));
        //Log.d("size", String.valueOf(materialInfo.size()));
        //Log.d("materials", String.valueOf(materialInfo));
        if(materialInfo.size()!=0) {
           String str = "";
            for (String matt : materialInfo) {
                str += matt;
            }
            //Log.d("str",str);
            mat = str.split("_");
            //Log.d("matSize", String.valueOf(mat.length));
            /*for(int j=0;j<mat.length;j++) {
                Log.d("mat", mat[j]);
            }*/
            for(int j=0;j<Dingredient.size();j++) {
                for (int i = 0; i < mat.length; i++) {
                    //Log.d("matting",mat[i]);
                    if (mat[i].contains(Dingredient.get(j))) {
                        //Log.d("Find!!!!","Yeah");
                        Intent intent = new Intent(getApplicationContext(), MainActivity.class);
                        intent.putExtra("foodName", foodName.get(i));
                        intent.putExtra("foodImage", foodImage.get(i));
                        Log.d("material",mat[i]);
                        intent.putExtra("materialInfo", mat[i]);
                        intent.putExtra("recipeOrder", recipeOrder.get(i));
                        intent.putExtra("calorieInfo", calorieInfo.get(i));
                        intent.putExtra("carbohydratesInfo", carbohydratesInfo.get(i));
                        intent.putExtra("proteinInfo", proteinInfo.get(i));
                        intent.putExtra("lipidInfo", lipidInfo.get(i));
                        intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                        startActivity(intent);
                        return;
                    }
                }
            }
        }
        startService(new Intent(this, SelectDiet.class));
    }
}