package com.example.dictionary;

import androidx.appcompat.app.AppCompatActivity;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentTransaction;

import android.os.Bundle;

public class MainActivity extends AppCompatActivity implements WordFragment.OnWordSelectedListener  {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        if(findViewById(R.id.fragment_container)!=null){
            WordFragment wordFragment = new WordFragment();
            FragmentManager fm = getSupportFragmentManager();
            fm.beginTransaction().add(R.id.fragment_container,wordFragment).commit();
        }
    }
    public void onWordSelected(int position) {
        if (findViewById(R.id.fragment_container) == null) {
            DefinitionFragment defFrag = (DefinitionFragment) getSupportFragmentManager()
                    .findFragmentById(R.id.definition_fragment);
            defFrag.updateDefinitionView(position);
        } else {
            DefinitionFragment newFragment = new DefinitionFragment();
            Bundle args = new Bundle();
            args.putInt(DefinitionFragment.ARG_POSITION, position);
            newFragment.setArguments(args);
            FragmentTransaction transaction = getSupportFragmentManager().beginTransaction();
            transaction.replace(R.id.fragment_container, newFragment);
            transaction.addToBackStack(null);
            transaction.commit();
        }
    }
}