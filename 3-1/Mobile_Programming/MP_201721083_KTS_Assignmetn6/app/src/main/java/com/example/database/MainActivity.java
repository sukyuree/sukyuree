package com.example.database;

import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteException;
import android.database.sqlite.SQLiteOpenHelper;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.ListView;

import androidx.appcompat.app.AppCompatActivity;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;

class DBHelper extends SQLiteOpenHelper {
    private static final String DATABASE_NAME = "spheregeom.db";
    private static final int DATABASE_VERSION = 1;

    public DBHelper(Context context) {
        // invoke super constructor.

        super(context, DATABASE_NAME, null,DATABASE_VERSION);

    }
    @Override
    public void onCreate(SQLiteDatabase db) {
        // create two tables, vertices and triangles
        db.execSQL("create table vertices (_id integer primary key autoincrement, x float, y float, z float)");
        db.execSQL("create table triangles (_id integer primary key autoincrement, v0 integer, v1 integer, v2 integer)");
    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        // drop the two tables, vertices and triangles
        db.execSQL("drop table if exists vertices;");
        db.execSQL("drop table if exists triangles;");

        onCreate(db);
    }
}

public class MainActivity extends AppCompatActivity {
    private SQLiteDatabase  db;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // create a DBHelper instance
        // obtain a database object from the DBHelper instance.

        DBHelper helper = new DBHelper(this);
        try{
            db = helper.getWritableDatabase();
        }
        catch(SQLiteException e){
            db = helper.getReadableDatabase();
        }
    }

    public void readSphere(View view) {
        // delete all the records from vertices.
        // delete all the records from triangles.

        db.execSQL("delete from vertices");
        db.execSQL("delete from triangles");

        // open the file R.raw.sphere and get InputStream instance
        // Create a BufferedReader instance
        
        InputStream inputStream = this.getResources().openRawResource(R.raw.sphere);
        BufferedReader buffreader = new BufferedReader(new InputStreamReader(inputStream));

        String line;

        // Using the readLine() method, read each line from the file and
        // insert the data into the associated table. That is,
        // "v 0.1 0.3 2.5" --> insert into vertices values (null, 0.1, 0.3, 2.5);
        // "f 10 3 23" --> insert into triangles value (null, 10, 3, 23);

        try{
            while ((line = buffreader.readLine())!=null){
                String[] tokens = line.split(" ");
                if(tokens[0].equals("v")){
                    db.execSQL("INSERT INTO vertices VALUES (null,'"+tokens[1] + "','"+tokens[2]+"','"+tokens[3]+"');");
                }
                else if(tokens[0].equals("f")){
                    db.execSQL("INSERT INTO triangles VALUES (null,'"+tokens[1] + "','"+tokens[2]+"','"+tokens[3]+"');");
                }
            }
        }
        catch(IOException e){

        }

    }
    public void load_list(View view) {
        Cursor cursor;
        Cursor curs;
        // select vertices with x less than -6 from the table vertices
        // take the three values, x, y, z from each record and output them with Log.v().
        ListView listview = (ListView)findViewById(R.id.sphere_list);
        ArrayList<String> list = new ArrayList<>();
        cursor = db.rawQuery("SELECT x,y,z FROM vertices ;",null);
        while (cursor.moveToNext()) {
            String x = cursor.getString(0);
            String y = cursor.getString(1);
            String z = cursor.getString(2);
            String line = "vertex pos (" + x + "," + y + "," + z + ")";
            list.add(line);
        }
        curs = db.rawQuery("SELECT v0,v1,v2 FROM triangles ;",null);
        while (cursor.moveToNext()) {
            String v0 = curs.getString(0);
            String v1 = curs.getString(1);
            String v2 = curs.getString(2);
            String line = "triangle pos (" + v0 + "," + v1 + "," + v2 + ")";
            list.add(line);
        }
//        cursor = db.rawQuery("SELECT x, y, z FROM triangles WHERE x >1560;",null);
//        while (cursor.moveToNext()){
//            String x = cursor.getString(0);
//            String y = cursor.getString(1);
//            String z = cursor.getString(2);
//            String line = "vertex pos ("+x+","+y+","+z+")";
//            list.add(line);
//        }
        ArrayAdapter ad = new ArrayAdapter(this,android.R.layout.simple_list_item_1,list);
        listview.setAdapter(ad);
    }

    public void queryDatabase(View view) {
        Cursor cursor;

        // select vertices with x less than -6 from the table vertices
        // take the three values, x, y, z from each record and output them with Log.v().

        cursor = db.rawQuery("SELECT x, y, z FROM vertices WHERE x <-6;",null);
        while (cursor.moveToNext()){
            String x = cursor.getString(0);
            String y = cursor.getString(1);
            String z = cursor.getString(2);
            Log.v("OUT","vertex pos ("+x+","+y+","+z+")");

        }
    }

}
