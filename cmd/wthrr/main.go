package main

import (
	"fmt"
	"log"
	"os"
)

const version = "dev"

func main() {
	if len(os.Args) > 1 && os.Args[1] == "version" {
		fmt.Printf("wthrr version %s\n", version)
		return
	}

	fmt.Println("wthrr - Weather simulation application")
	fmt.Println("This is a Go port of the wthrr weather effects application.")
	fmt.Printf("Version: %s\n", version)
	
	// TODO: Implement weather simulation logic
	log.Println("Weather simulation would start here...")
}