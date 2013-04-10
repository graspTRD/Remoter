package com.pccontroler.protocol;

import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import com.pccontroler.action.PCControlerAction;

public abstract class PCControlerConnection
{
	public static final String BLUETOOTH_UUID = "300ad0a7-059d-4d97-b9a3-eabe5f6af813";
	public static final String DEFAULT_PASSWORD = "aa12";
	
	private DataInputStream dataInputStream;
	private OutputStream outputStream;
	
	public PCControlerConnection(InputStream inputStream, OutputStream outputStream)
	{
		this.dataInputStream = new DataInputStream(inputStream);
		this.outputStream = outputStream;
	}
	
	public PCControlerAction receiveAction() throws IOException
	{
		synchronized (this.dataInputStream)
		{
			PCControlerAction action = PCControlerAction.parse(this.dataInputStream);
			return action;
		}
	}
	
	public void sendAction(PCControlerAction action) throws IOException
	{
		ByteArrayOutputStream baos = new ByteArrayOutputStream();
		action.toDataOutputStream(new DataOutputStream(baos));
		
		synchronized (this.outputStream)
		{
			this.outputStream.write(baos.toByteArray());
			this.outputStream.flush();
		}
	}
	
	public void close() throws IOException
	{
		this.dataInputStream.close();
		this.outputStream.close();
	}
}
