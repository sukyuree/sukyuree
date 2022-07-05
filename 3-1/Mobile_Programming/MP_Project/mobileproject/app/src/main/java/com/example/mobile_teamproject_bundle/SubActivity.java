//for recipe

package com.example.mobile_teamproject_bundle;

import android.content.Intent;
import android.graphics.Color;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.ListAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;
import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserException;
import org.xmlpull.v1.XmlPullParserFactory;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLEncoder;
import java.util.ArrayList;
import java.util.HashMap;

import java.io.*;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLEncoder;
import java.util.HashMap;
import java.util.Map;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

public class SubActivity extends AppCompatActivity {

    String str;
    String keyword;
    ListView mProductListview;
   // PriceListAdapter adapter;
    PriceListParser naverSearchPaser = new PriceListParser();
    ArrayList<PriceDataStruct> Data = new ArrayList<PriceDataStruct>();
    PriceDataStruct xmlData = new PriceDataStruct();
    /*ArrayList<String> result_title_list;
    ArrayList<String> result_link_list;
    ArrayList<Integer> result_price_list;
    ArrayList<String> result_img_list;*/
    String[] titles = new String[60];
    Integer[] prices = new Integer[60];
    String[] markets = new String[60];
    String[] images = new String[60];
    String[] links = new String[60];
    String[] ingredients;

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.recipe);

        Intent intent = getIntent();
        String foodNm = intent.getStringExtra("name");
        String recipe = intent.getStringExtra("recipe");
        String word = intent.getStringExtra("ingredients");
        String image = intent.getStringExtra("image");
        String category = intent.getStringExtra("info");

        TextView Recipe = (TextView)findViewById(R.id.food_recipe);
        Recipe.setText(recipe);
        TextView FN = (TextView)findViewById(R.id.foodNm);
        FN.setText(foodNm);
        ImageView img = (ImageView)findViewById(R.id.imageView4);
        TextView cat = (TextView)findViewById(R.id.textView9);
        //cat.setText("calorie: "+category);

        if(image == null){
          img.setImageResource(R.drawable.noimg);
        }
        else GlideApp.with(this).load(image).into(img);
       //ingredients에 재료 정보 넣기 일단 임의로
        word = word.replaceAll(" ","");
        //Log.d("재료",word);
        ingredients = word.split(",");

        IngredientAdapter Adapter  = new
                IngredientAdapter(this,ingredients);//,foodNm,recipe);

        ListView listview = (ListView) findViewById(R.id.ingredient_list);
        listview.setAdapter((ListAdapter) Adapter);

        listview.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                //startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse(links[position])));
                keyword = ingredients[position];  //keyword: 시금치
                Log.d("keyword",keyword);
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        getNaverSearch(keyword);
                    }
                }).start();
                setContentView(R.layout.list);
            }
        });
    }

    public void getNaverSearch(String keyword) {
        String clientId = "NjG4PNpPgpJuqszwoz2m";
        String clientSecret = "qzKVyrvfUk";

        String text = null;
        try {
            text = URLEncoder.encode(keyword, "UTF-8");
        } catch (UnsupportedEncodingException e) {
            throw new RuntimeException("검색어 인코딩 실패", e);
        }


        String apiURL = "https://openapi.naver.com/v1/search/shop.xml?query=" + text + "&display=50" + "&start=1" + "&sort=asc";    // json 결과
        //String apiURL = "https://openapi.naver.com/v1/search/shop.xml?query="+ text; // xml 결과


        Map<String, String> requestHeaders = new HashMap<>();
        requestHeaders.put("X-Naver-Client-Id", clientId);
        requestHeaders.put("X-Naver-Client-Secret", clientSecret);
        String responseBody = get(apiURL, requestHeaders);
        for(int i=0;i<Data.size();i++){
            //strArr[i] = Data.get(i).getTitle();
            titles[i] = Data.get(i).getTitle();
            images[i] = Data.get(i).getImage();
            prices[i] = Data.get(i).getLprice();
            markets[i] = Data.get(i).getMallName();
            links[i] = Data.get(i).getLink();
        }
        CustomListAdapter adapter  = new
                CustomListAdapter(this,titles,images,markets,prices);
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                //Log.d("tag", findViewById(R.id.productListview).toString());
                ListView listview = (ListView) findViewById(R.id.productListview);
                listview.setAdapter(adapter);
                //Log.d("tag", String.valueOf(listview));
                listview.setOnItemClickListener(new AdapterView.OnItemClickListener() {
                    @Override
                    public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                        //Log.d("tag",links[position]);
                        startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse(links[position])));
                    }
                });
            }
        });
    }

            private String get(String apiUrl, Map<String, String> requestHeaders) {
                HttpURLConnection con = connect(apiUrl);
                try {
                    con.setRequestMethod("GET");
                    for (Map.Entry<String, String> header : requestHeaders.entrySet()) {
                        con.setRequestProperty(header.getKey(), header.getValue());
                    }
                    int responseCode = con.getResponseCode();
                    if (responseCode == HttpURLConnection.HTTP_OK) { // 정상 호출
                        return readBody(con.getInputStream());
                    } else { // 에러 발생
                        return readBody(con.getErrorStream());
                    }
                } catch (IOException | ParserConfigurationException e) {
                    throw new RuntimeException("API 요청과 응답 실패", e);
                } finally {
                    con.disconnect();
                }
            }


    private HttpURLConnection connect(String apiUrl) {
        try {
            URL url = new URL(apiUrl);
            //Log.d("tag",naverSearchPaser.GetXmlData(url,"컴퓨터").get(0).getTitle());
            return (HttpURLConnection)url.openConnection();
        } catch (MalformedURLException e) {
            throw new RuntimeException("API URL이 잘못되었습니다. : " + apiUrl, e);
        } catch (IOException e) {
            throw new RuntimeException("연결이 실패했습니다. : " + apiUrl, e);
        }
    }



    private String readBody(InputStream body) throws ParserConfigurationException {
        InputStreamReader streamReader = new InputStreamReader(body);

        try (BufferedReader lineReader = new BufferedReader(streamReader)) {
            StringBuilder responseBody = new StringBuilder();

            Data = naverSearchPaser.GetXmlData(body,keyword);

            String line;
            while ((line = lineReader.readLine()) != null) {
                responseBody.append(line);
            }

            return responseBody.toString();
        } catch (IOException e) {
            throw new RuntimeException("API 응답을 읽는데 실패했습니다.", e);
        }
    }
}
