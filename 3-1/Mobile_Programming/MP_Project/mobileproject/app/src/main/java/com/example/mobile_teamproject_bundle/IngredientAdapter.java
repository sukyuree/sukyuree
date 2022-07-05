package com.example.mobile_teamproject_bundle;

import android.app.Activity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.TextView;

public class IngredientAdapter extends ArrayAdapter {
    private final Activity context;
    String[] titles = new String[60];
    String name;
    String rec;

    public IngredientAdapter(Activity context,String[] object){//,String Fname,String recipe) {
        super(context, R.layout.ingredients, object);
        titles = object;  //title에는 재료정보
        //name = Fname;
        //rec = recipe;
        this.context = context;
    }
    
    public View getView(int position, View view, ViewGroup parent) {
        LayoutInflater inflater = context.getLayoutInflater();
        View rowView= inflater.inflate(R.layout.ingredients, null, false);
        TextView title = (TextView) rowView.findViewById(R.id.ingredients);
        title.setText(titles[position]);
        return rowView;
    }
}
