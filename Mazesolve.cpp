

#include <iostream>
#include "MazeSolver.h"
#include <fstream>

//using namespace std;

MazeSolver::MazeSolver(std::string input_file){
    std::ifstream i_file;
    i_file.open(input_file);
    if(!i_file)
    {
        std::cerr << "Cannot read from input_file_name" << std::endl;
        exit(1);
    }
    
    
        while(i_file >> maze_rows_ >> maze_columns_){
    
    
    initializeMaze(maze_rows_, maze_columns_);
    fillMaze(i_file);
    initializeSolution();
}
    maze_ready = true;
    i_file.close();

}
//need to work on

bool MazeSolver::solveMaze(){
    direction dir;
    Position current_position = {0,0};
    backtrack_stack_.push(current_position);

    while (!backtrack_stack_.empty()) {
        current_position = backtrack_stack_.top();
        int curRow = current_position.row;
        int curCol = current_position.column;
        if (solution_[curRow][curCol] == '$' ) {
            return true;
        } else if (extendPath(current_position)) {
            solution_[curRow][curCol] = '>';
            current_position = backtrack_stack_.top();
            }
             else 
            {
                maze_[curRow][curCol] = 'X';
                solution_[curRow][curCol] = '@';
                backtrack_stack_.pop();
                if (!backtrack_stack_.empty()) 
                {
                    current_position = backtrack_stack_.top();
            } 
            else {

                std::cout << "This maze has no solution." << std::endl;
            }
        }
        
    }
    return false;
}


MazeSolver::~MazeSolver(){
    if((maze_!=nullptr) && (solution_!=nullptr)){
        
        for( int i =0 ; i< maze_rows_ ; ++i){
            delete[] maze_[i];
            delete[] maze_;
        }

        for( int i =0 ; i< maze_rows_ ; ++i){
            delete[] solution_[i];
            delete[] solution_;
        }
    }
}

bool MazeSolver::mazeIsReady(){
    return maze_ready;
}

bool MazeSolver::extendPath(Position current_position){
    bool extended = false;
    if (isExtensible(current_position, SOUTH)) {
        backtrack_stack_.push( getNewPosition(current_position,SOUTH) );
        extended = true;
    }
    if (isExtensible(current_position, EAST)) {
        backtrack_stack_.push( getNewPosition(current_position,EAST) );
        extended = true;
    }
    return extended;
}

bool MazeSolver::isExtensible(Position current_position, direction dir) {
    int curRow = current_position.row;
    int curCol = current_position.column;
    bool extensible = false;


    if (dir == SOUTH) {
        int newRow = curRow+1;
        int newCol = curCol;

        std::cout << current_position.row << " " << current_position.column << std::endl;
        
        if (curRow < maze_rows_-1) {
            if (solution_[newRow][newCol] == '_' || solution_[newRow][newCol] == '$') {
                extensible = true;
            }
        } 
    } else if (dir == EAST) {
        int newRow = curRow;
        int newCol = curCol+1;
        std::cout << "\nCurrent Position: " << current_position.row << " " << current_position.column << std::endl;
        if (curCol < maze_columns_-1) {
            if (solution_[newRow][newCol] == '_' || solution_[newRow][newCol] == '$') {
                extensible = true;
            }
        } 
    } else {
        std::cout << "Error! Enter a valid direction.\n";
    }
    return extensible;
}


void MazeSolver::initializeMaze(int rows, int columns) {
    //std::ifstream input(input_file);
    maze_ = new char *[rows]; //Creating Dynamic Matrix.
    //char z=' '; // creating one variable to get the char from .txt
    for (int i=0; i<rows; i++)
    {
        maze_[i]= new char[columns];
    }
 
}

void MazeSolver::fillMaze(std::ifstream& input_stream){
   
            
            for(int i = 0; i < maze_rows_; i++){
                for(int j = 0; j < maze_columns_; j++){
                    input_stream >> maze_[i][j];
                }
            }
}

Position MazeSolver::getNewPosition(Position old_position, direction dir){
    int old_position_row = old_position.row;
    int old_position_col = old_position.column;

    if (dir == SOUTH) {
        Position new_position;
        new_position.row = old_position_row+1;
        new_position.column = old_position_col;
        std::cout << "New Position: " << new_position.row << " " << new_position.column << std::endl;
        return new_position;
    } else if (dir == EAST) {
        Position new_position;
        new_position.row = old_position_row;
        new_position.column = old_position_col+1;
        std::cout << "New Position: " << new_position.row << " " << new_position.column << std::endl;
        return new_position;
    }

return old_position;

}

//work on
// copy maze to solution...didnt impliemlent it
void MazeSolver::initializeSolution() {
    copyMazetoSolution();
        Position startPosition;
        startPosition.row = 0;
        startPosition.column = 0;
        solution_[startPosition.row][startPosition.column] = '>';
        extendPath(startPosition);

}
void MazeSolver::copyMazetoSolution(){
    solution_ = new char * [maze_rows_];
    for (int i=0; i<maze_rows_; i++) {
        solution_[i] = new char[maze_columns_];
    }
        //std::cout << std::endl;
    for (int i=0; i<maze_rows_; i++) {
        for (int j=0; j<maze_columns_; j++) {
            solution_[i][j] = maze_[i][j];
        }
    }
}



void MazeSolver::printSolution(){
    std::cout << "\nThe solution to this maze is:\n";
    for (int i=0; i<maze_rows_; i++) {
        for (int j=0; j<maze_columns_; j++) {
            std::cout << solution_[i][j];
        }
        std::cout << std::endl << i;
    }

}
