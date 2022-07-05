package com.example.mobile_teamproject_bundle;

import android.app.Activity;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ProgressBar;
import android.widget.RatingBar;
import android.widget.TextView;
import android.widget.Toast;

public class exerciseListAdapter extends ArrayAdapter {
    private final Activity context;
    String[] exercises;
    int[] rating;
    //private boolean see = false;

    public  exerciseListAdapter(Activity context, String[] object,int[] rate){//,boolean watch) {
        super(context, R.layout.detail, object);
        this.context = context;
        exercises = object;
        //see = watch;
        rating = rate;
    }
    @Override
    public View getView(int position, View view, ViewGroup parent) {
        LayoutInflater inflater = context.getLayoutInflater();
        View rowView= inflater.inflate(R.layout.rating_exercise, null, false);
        TextView exercise = (TextView) rowView.findViewById(R.id.textView5);
        exercise.setText(exercises[position]);
        if(rating.length!=0){
          ProgressBar pro = (ProgressBar)rowView.findViewById(R.id.progressBar);
          if (rating[position]< 0 || rating[position] > 100) {
            Log.d("rating","invalidate");
          } else {
            // 변환된 값을 프로그레스바에 적용.
            pro.setProgress(rating[position]) ;
          }
        }
        return rowView;
    }
}
