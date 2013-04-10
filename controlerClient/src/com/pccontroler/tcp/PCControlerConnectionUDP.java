package com.pccontroler.tcp;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;

import com.pccontroler.action.BroadcastAction;

public class PCControlerConnectionUDP
{
	public final static int DEFAULT_PORT = 53399;
	
	public final static String BROADCAST_ADDRESS = "255.255.255.255";
	
	private int port = 53399;
	
	private DatagramSocket dgSocket;
	
	public PCControlerConnectionUDP(int port) throws SocketException
	{
		this.port = port;
		dgSocket = new DatagramSocket(port);
	}
	
	public void sendAction() throws IOException
	{
		// ByteArrayOutputStream baos = new ByteArrayOutputStream();
		
		// action.toDataOutputStream(new DataOutputStream(baos));
		
		InetAddress serverAddress = InetAddress.getByName(this.BROADCAST_ADDRESS);
		
		String str = "pccontroler"; // 这是要传输的数据
		
		byte data2[] = str.getBytes(); // 把传输内容分解成字节
		
		short length = (short) str.length();
		
		byte[] data1 = new byte[2];
		data1[1] = (byte) (length & 0xff);
		data1[0] = (byte) ((length >> 8) & 0xff);
		
		byte[] buf = new byte[data1.length + data2.length];
		System.arraycopy(data1, 0, buf, 0, data1.length);
		System.arraycopy(data2, 0, buf, data1.length, data2.length);
		
		DatagramPacket packet = new DatagramPacket(buf, buf.length, serverAddress, port);
		
		this.dgSocket.send(packet);
	}
	
	public BroadcastAction receiveAction() throws IOException
	{
		
		byte[] by = new byte[1024];
		
		DatagramPacket dgPacket = new DatagramPacket(by, by.length);
		
		dgSocket.receive(dgPacket);
		
		String adrAddress = dgPacket.getAddress().toString().replace("/", "");
		
		int port = dgPacket.getPort();
		
		String host = new String(dgPacket.getData(), dgPacket.getOffset(), dgPacket.getLength());
		
		if (host.contains("pccontroler"))
			
			return null;
		
		// if (host.equalsIgnoreCase("pccontrol"))
		//
		// continue;
		
		BroadcastAction action = new BroadcastAction(host, adrAddress, port);
		
		return action;
		
	}
	
	public void close()
	{
		this.dgSocket.close();
	}
	
}
