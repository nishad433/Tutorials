package main

import (
	"flag"
	"github.com/nishad433/chat/chat"
	"log"
	"net"
)

func main() {

	port := flag.String("port", "8080", "TCP Port to listen on")
	flag.Parse()

	s := chat.NewServer()
	go s.Run()

	listner, err := net.Listen("tcp", ":"+*port)

	if err != nil {
		log.Fatalf("Unable to listen on %s error:%s", *port, err)
	}

	defer listner.Close()

	log.Printf("Listening on Port %s\n", *port)

	for {
		conn, err := listner.Accept()

		if err != nil {
			log.Fatalf("Unable to accept connection")
			continue
		}

		go s.NewClient(conn)

	}

}
