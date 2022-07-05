package com.example.mobile_teamproject_bundle;

import android.app.AlarmManager;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.AdapterView;
import android.widget.ListAdapter;
import android.widget.ListView;
import android.widget.ProgressBar;
import android.widget.RatingBar;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.NotificationCompat;

import java.util.ArrayList;
import java.util.Calendar;

public class ExercisingActivity extends AppCompatActivity {
    private static final int NOTIFICATION_ID = 13;
    public static final String PREFS_NAME = "MyPrefs";
    String search = "하체운동";
    //ArrayList<String> arrayList = (ArrayList<String>) intent.getSerializableExtra("recommended");
    String[] exercise  = {"하체운동","상체운동","허리운동","요가"};
    int Size;
    int[] rating = new int[exercise.length];
    boolean IsNewR = false;
    boolean watch;
    int count=0;
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        SharedPreferences settings = getSharedPreferences(PREFS_NAME, 0);

        Intent in = getIntent();
        exercise  = in.getStringArrayExtra("recommended");
        search = in.getStringExtra("운동");
        IsNewR = in.getBooleanExtra("IsNew",true);
        Log.d("IsNew?", String.valueOf(IsNewR));

        if(!IsNewR) {
            Size = settings.getInt("size", 0);
            for (int j = 0; j < Size; j++) {
                rating[j] = settings.getInt(String.valueOf(j), 0);
                if(rating[j]!=100){
                    count++;
                }
            }
        }

        if(count!=0){
            sendNotification();
        }
        setContentView(R.layout.exercise_video);

        //watching(search);
        //rating++;

        watching(search);

        for(int u =0 ;u<exercise.length;u++) {
            Log.d("exercise", exercise[u]);
        }

        exerciseListAdapter Adapter  = new
                exerciseListAdapter(this, exercise,rating);

        ListView listview = (ListView) findViewById(R.id.ex_list);
        listview.setAdapter((ListAdapter) Adapter);

        listview.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                search = exercise[position];
                watching(search);
                //rating+=20;
                rating[position]+=20;
                ProgressBar pro = (ProgressBar)view.findViewById(R.id.progressBar);
                if (rating[position]< 0 || rating[position] > 100) {
                    Log.d("rating","invalidate");
                } else {
                    // 변환된 값을 프로그레스바에 적용.
                    pro.setProgress(rating[position]) ;
                }
            }
        });
    }

    public void sendNotification(){
        AlarmManager alarmManager = (AlarmManager) this.getSystemService(Context.ALARM_SERVICE);
        Intent receiverIntent = new Intent(this, AlarmRecevier.class);
        PendingIntent pIntent = PendingIntent.getBroadcast(this, 0, receiverIntent, 0);
        //alarmManager.cancel(pIntent);
        if (alarmManager!= null) {
            Log.d("alarm","12");
            alarmManager.cancel(pIntent);
        }

        Calendar calendar = Calendar.getInstance();
        calendar.setTimeInMillis(System.currentTimeMillis());

        if(Calendar.getInstance().after(calendar)){
            calendar.add(Calendar.DAY_OF_MONTH,1);
        }

        //Log.d("exc", String.valueOf(calendar.getTime()));
        alarmManager.set(AlarmManager.RTC_WAKEUP, calendar.getTimeInMillis()+(3 * 60 * 60 * 1000),pIntent);//InexactRepeating(AlarmManager.RTC_WAKEUP, calendar.getTimeInMillis(), AlarmManager.INTERVAL_DAY, pIntent);
    }

        public void watching(String key){
        WebView mWebView = (WebView) findViewById(R.id.webView2);

        mWebView.setWebViewClient(new WebViewClient()); // 클릭시 새창 안뜨게
        WebSettings mWebSettings = mWebView.getSettings(); //세부 세팅 등록
        mWebSettings.setJavaScriptEnabled(true); // 웹페이지 자바스클비트 허용 여부
        mWebSettings.setSupportMultipleWindows(false); // 새창 띄우기 허용 여부
        mWebSettings.setJavaScriptCanOpenWindowsAutomatically(false); // 자바스크립트 새창 띄우기(멀티뷰) 허용 여부
        mWebSettings.setLoadWithOverviewMode(true); // 메타태그 허용 여부
        mWebSettings.setUseWideViewPort(true); // 화면 사이즈 맞추기 허용 여부
        mWebSettings.setSupportZoom(false); // 화면 줌 허용 여부
        mWebSettings.setBuiltInZoomControls(false); // 화면 확대 축소 허용 여부
        mWebSettings.setLayoutAlgorithm(WebSettings.LayoutAlgorithm.SINGLE_COLUMN); // 컨텐츠 사이즈 맞추기
        mWebSettings.setCacheMode(WebSettings.LOAD_NO_CACHE); // 브라우저 캐시 허용 여부
        mWebSettings.setDomStorageEnabled(true); // 로컬저장소 허용 여부

        mWebView.loadUrl("https://www.youtube.com/results?search_query="+ key);
    }

   public void onStop(){
       super.onStop();
       SharedPreferences settings = getSharedPreferences(PREFS_NAME, 0);
       SharedPreferences.Editor editor = settings.edit();
       Intent intent = new Intent(getApplicationContext(),ExercisePlanActivity.class);
       //editor.putBoolean("back",true);
       editor.putInt("size",rating.length);
       intent.putExtra("sizw",rating.length);
       intent.putExtra("exercise",exercise);
       for(int i=0;i<rating.length;i++) {
           //Log.d("save", String.valueOf(rating[i]));
           editor.putInt(String.valueOf(i), rating[i]);
           intent.putExtra(String.valueOf(i),rating[i]);
       }
       editor.commit();
   }

}
