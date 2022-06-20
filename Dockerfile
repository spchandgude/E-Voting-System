FROM gcc:latest

COPY . /usr/src/OnlineVotingSystem

WORKDIR /usr/src/OnlineVotingSystem

RUN g++ -o Test *.cpp -l sqlite3 

CMD [ "./Test" ]

