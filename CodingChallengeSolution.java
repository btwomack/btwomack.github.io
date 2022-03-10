package com.codingchallange.solution;

public class CodingChallengeSolution {

    public static void main(String[] args) {

        int limit = 4000000;
        int sum = 0;
        int a = 1;
        int b = 2;
        int temp = 0;


        while (b < limit) {
            if (b % 2 == 0) {

                sum += b;
            }
            temp = a;
            a = b;
            b = a + temp;
        }
                System.out.println(sum);

        }
    }









