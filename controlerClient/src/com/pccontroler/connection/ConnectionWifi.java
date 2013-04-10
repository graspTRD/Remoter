package com.pccontroler.connection;

import java.io.IOException;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;

import com.pccontroler.activity.connection.ConnectionWifiEditActivity;
import com.pccontroler.app.PCControler;
import com.pccontroler.protocol.PCControlerConnection;
import com.pccontroler.tcp.PCControlerConnectionTcp;

public class ConnectionWifi extends Connection
{
	private String host;
	private int port;
	
	public ConnectionWifi()
	{
		super();
		
		this.host = "192.168.0.1";
		this.port = PCControlerConnectionTcp.DEFAULT_PORT;
	}
	
	public static ConnectionWifi load(SharedPreferences preferences, int position)
	{
		ConnectionWifi connection = new ConnectionWifi();
		
		connection.host = preferences.getString("connection_" + position + "_host", null);
		
		connection.port = preferences.getInt("connection_" + position + "_port", 0);
		
		return connection;
	}
	
	public void save(Editor editor, int position)
	{
		super.save(editor, position);
		
		editor.putInt("connection_" + position + "_type", WIFI);
		
		editor.putString("connection_" + position + "_host", this.host);
		
		editor.putInt("connection_" + position + "_port", this.port);
	}
	
	public void edit(Context context)
	{
		Intent intent = new Intent(context, ConnectionWifiEditActivity.class);
		this.edit(context, intent);
	}
	
	public PCControlerConnection connect(PCControler application) throws IOException
	{
		return PCControlerConnectionTcp.create(this.host, this.port);
	}
	
	public String getHost()
	{
		return host;
	}
	
	public void setHost(String host)
	{
		this.host = host;
	}
	
	public int getPort()
	{
		return port;
	}
	
	public void setPort(int port)
	{
		this.port = port;
	}
}
