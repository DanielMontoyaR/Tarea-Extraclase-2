#include "pch.h"
#include <WinSock2.h>
#include <iostream>
#pragma warning(disable : 4996)
using namespace std;


SOCKET TCPClientSocket;


struct Server {

	//Variables locales para testear
	string mensajeEnviado="";
	int numeroConfirmacion = 0;
	bool mensajeRecibido;


	Server() {
	}

	//Creación de constructores que inicializan las variables locales para testear.

	explicit Server(const string mensajeEnviado) 
		: mensajeEnviado{ mensajeEnviado } {
	}

	explicit Server(const int numeroConfirmacion)
		: numeroConfirmacion{ numeroConfirmacion } {
	}

	explicit Server(const bool mensajeRecibido)
		: mensajeRecibido{ mensajeRecibido } {
	}


	void InitClient() { //Funcion de inicialización del cliente 

		WSADATA WinSockData;
		int		iWsaStartup;

		iWsaStartup = WSAStartup(MAKEWORD(2, 2), &WinSockData);

		if (iWsaStartup==0) {
			numeroConfirmacion = iWsaStartup;
		}
		else {
			numeroConfirmacion = -100;
		}
	}


	void CreaSocket() {//Funcion de inicialización del socket 
		
		TCPClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (TCPClientSocket == INVALID_SOCKET) {
			numeroConfirmacion = -1;
		}
		else {
			numeroConfirmacion = 0;
		}

	}


	void ConexionServidor() {//Funcion de conexión con el servidor

		int iConnect;

		struct sockaddr_in TCPServerAdd;

		TCPServerAdd.sin_family = AF_INET;
		TCPServerAdd.sin_addr.s_addr = inet_addr("127.0.0.1");
		TCPServerAdd.sin_port = htons(8000);

		iConnect = connect(TCPClientSocket, (SOCKADDR*)&TCPServerAdd, sizeof(TCPServerAdd));

		if (iConnect == SOCKET_ERROR) {
			numeroConfirmacion = -1;
		}
		else {
			numeroConfirmacion = 0;
		}
	}

	void ReceptionData() {//Funcion de recepción de información por parte del servidor

		int iRecv;
		char RecvBuffer[512];
		int iRecvBuffer = strlen(RecvBuffer) + 1;

		iRecv = recv(TCPClientSocket, RecvBuffer, iRecvBuffer, 0);

		if (iRecv == SOCKET_ERROR) {
			mensajeRecibido = false;
		}
		else {

			mensajeRecibido = true;
		}

	}

	void EnviarData() {//Funcion de envío de información por parte del cliente

		int iSend;
		char SenderBuffer[512] = "Saludos desde el cliente";
		int iSenderBuffer = strlen(SenderBuffer) + 1;

		iSend = send(TCPClientSocket, SenderBuffer, iSenderBuffer, 0);

		if (iSend == SOCKET_ERROR) {
			mensajeEnviado = "Error";
		}
		else {
			mensajeEnviado = SenderBuffer;
		}
	}


	void CierreSocket() { //Funcion de cierre del socket 

		int iCloseSocket;

		iCloseSocket = closesocket(TCPClientSocket);

		if (iCloseSocket == SOCKET_ERROR) {
			numeroConfirmacion = -1;
		}
		else {
			numeroConfirmacion = 0;
		}
		
	}

	void Cleanup() {//Funcion de cleanup

		int	iWsaCleanup;

		iWsaCleanup = WSACleanup();

		if (iWsaCleanup == SOCKET_ERROR) {
			numeroConfirmacion = -1;
		}
		else {
			numeroConfirmacion = 0;
		}

		
	}
	
};



TEST(ServidorTest, testInicio) {//Prueba para verificar el inicio exitoso del cliente

	Server pruebaInicio; //Mock object para testear la inicialización del cliente.

	pruebaInicio.InitClient();

	EXPECT_EQ(0, pruebaInicio.numeroConfirmacion);
}

TEST(ServidorTest, testSocket) {//Prueba para verificar el inicio exitoso del socket del cliente

	Server pruebaSocket;

	pruebaSocket.CreaSocket();

	EXPECT_EQ(0, pruebaSocket.numeroConfirmacion);
}

TEST(ServidorTest, testConexion) {//Prueba para verificar la conexión exitosa entre el cliente y el servidor.


	Server pruebaConexion;

	pruebaConexion.ConexionServidor();

	EXPECT_EQ(0, pruebaConexion.numeroConfirmacion);
}

TEST(ServidorTest, TestDataRecieved) {//Prueba para verificar que se han recibido datos exitosamente del servidor.

	Server pruebaDataRecieved;

	pruebaDataRecieved.ReceptionData();

	EXPECT_EQ(true, pruebaDataRecieved.mensajeRecibido);

}

TEST(ServidorTest, TestDataSent) {//Prueba para verificar que se han enviado datos exitosamente al servidor.

	Server pruebaDataSent;

	pruebaDataSent.EnviarData();

	EXPECT_EQ("Saludos desde el cliente", pruebaDataSent.mensajeEnviado);

}


TEST(ServidorTest, Cierre) {//Prueba para verificar que se ha cerrado exitosamente el Socket

	Server pruebaCierre;

	pruebaCierre.CierreSocket();

	EXPECT_EQ(0, pruebaCierre.numeroConfirmacion);

}

TEST(ServidorTest, CleanUp) {//Prueba para verificar que la función CleanUp se ejecutó exitosamente

	Server pruebaCleanUp;

	pruebaCleanUp.Cleanup();

	EXPECT_EQ(0, pruebaCleanUp.numeroConfirmacion);

}


int main(int argc, char* argv[]) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
