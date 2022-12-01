package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
);

func check(e error) {
    if e != nil {
        panic(e)
    }
}

func count(in []string, out chan int)  {
    //fmt.Println("Skal beregne fra " , in)
    tmpSum := 0;

    for _, val := range in {
        parsed, _ := strconv.Atoi(val);
        //fmt.Println("Parset verdi er ", parsed)
        //check(e);
        tmpSum += parsed;
    }

    out <- tmpSum;

}

func main() {
    //fmt.Println("hello world");
     f, err := os.Open("input")
     check(err);
     defer f.Close();
     scanner := bufio.NewScanner(f);
    blockCounts := make(chan int);

     var block []string = make([]string, 10);
    numberOfBlocks := 0;
     for scanner.Scan() {
        text := scanner.Text()
        //fmt.Print("Leste dette: ")
        //fmt.Println(text)
        if text == "" {
            //fmt.Print("SKal gå :)")
            go count(block, blockCounts);
            block = make([]string, 10);
            numberOfBlocks++;
        }else {
            block = append(block, text);
        }
     }
     max := -1;
     for true {
        read := <- blockCounts;
        if(read > max) {
            max = read;
        }
        
        numberOfBlocks--;
        if(numberOfBlocks == 0) {
            break;
        }
     }
     fmt.Println("Final result: ", max);

}