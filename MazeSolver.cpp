//constructor
//pre: input file is in correct format with first two values being integers
//      followed by valid maze characters in {'_', '*','$'}
//post: if inuput file cannot be read outputs "Cannot read from input_file"
//      otherwise sets maze_rows_ and maze_columns_ from the first two values in input file
//      and allocates two 2-dimesional array of size maze_rows_ and maze_columns_
//      both maze_ and solution_ are initialized with characters read from input
#include "MazeSolver.h"

MazeSolver::MazeSolver(std::string input_file) {
    std::ifstream myfile (input_file);
    if (myfile.is_open())
    {
        int rows, cols;
        myfile >> rows >> cols;
        initializeMaze(rows, cols);
        fillMaze(myfile);
        initializeSolution();
        copyMazetoSolution();

        maze_ready = true;
        myfile.close();
    }
    else
        std::cout << "Cannot read from "<< input_file;
}

MazeSolver::~MazeSolver() {
    if(mazeIsReady()){
        for(int i = 0;  i < maze_rows_ ; ++i){
            delete[] maze_[i];
            delete[] solution_[i];
        }
        delete[] maze_;
        delete[] solution_;
    }
}

void MazeSolver::initializeMaze(int rows, int columns) {
    maze_rows_ = rows;
    maze_columns_ = columns;
    maze_ = new char*[maze_rows_];
    for(int i=0; i<rows; i++){
        maze_[i] = new char[maze_columns_];
    }
}

bool MazeSolver::mazeIsReady() {
    return maze_ready;
}

void MazeSolver::fillMaze(std::ifstream &input_stream) {
    for(int i = 0;  i < maze_rows_ ; ++i){
        for(int j = 0; j < maze_columns_ ; ++j){
            char symbol;
            input_stream >> symbol;
            maze_[i][j] = symbol;
        }
    }
}

void MazeSolver::initializeSolution() {
    copyMazetoSolution();
    Position currentPosition;
    currentPosition.row = 0;
    currentPosition.column = 0;
    backtrack_stack_.push(currentPosition);
    solution_[currentPosition.row][currentPosition.column] = '>';

}

void MazeSolver::copyMazetoSolution() {
    solution_ = new char*[maze_rows_];
    for(int i = 0;  i < maze_rows_ ; ++i){
        solution_[i] = new char[maze_columns_];
        for(int j = 0; j < maze_columns_ ; ++j){
            solution_[i][j] = maze_[i][j];
        }
    }

}

bool MazeSolver::solveMaze() {
    Position currentPosition;
    currentPosition = backtrack_stack_.top();
    while(!backtrack_stack_.empty()){
        if(maze_[currentPosition.row][currentPosition.column] == '$'){
            std::cout<< "Found  the exit!!!"<< std::endl;
            std::cout << "The solution to this maze is: " << std::endl;
            return true;
        }
        else if(extendPath(currentPosition)){
            solution_[currentPosition.row][currentPosition.column] = '>';
            currentPosition = backtrack_stack_.top();
        }
        else{
            maze_[currentPosition.row][currentPosition.column] = 'X';
            solution_[currentPosition.row][currentPosition.column] = '@';
            backtrack_stack_.pop();
            if(!backtrack_stack_.empty()){
                currentPosition = backtrack_stack_.top();
            }
            else{
                std::cout << "This maze has no solution. " <<std::endl;
                std::cout << "The solution to this maze is: " << std::endl;

                return false;
            }

        }
    }
    return false;
}

bool MazeSolver::extendPath(Position current_position) {
    bool extended = false;
    if(isExtensible(current_position, SOUTH)){
        backtrack_stack_.push(getNewPosition(current_position,SOUTH));
        extended = true;
    }
    if(isExtensible(current_position,EAST)){
        backtrack_stack_.push(getNewPosition(current_position,EAST));
        extended = true;
    }
    return extended;
}
//bool isExtensible(Position current_position, direction dir); 
//(again, refer to the interface for pre and post conditions)
//A position is extensible if from the current position you can move either SOUTH or
//EAST by moving into a position marked as ‘ _ ‘ or ‘ $ ‘ on the maze_. Make sure you
//keep in mind the boundaries of the matrices here and don’t try to access indices
//outside of the maze_
bool MazeSolver::isExtensible(Position current_position, direction dir) {
    bool extensible = false;
    if(dir == SOUTH && current_position.row < maze_rows_ - 1){   //for south
        if(maze_[current_position.row + 1][current_position.column] == '$'
        || maze_[current_position.row + 1][current_position.column] == '_')
            extensible = true;
    }
    else if(dir == EAST && current_position.column < maze_columns_ - 1){       //for east
        if(maze_[current_position.row][current_position.column + 1] == '$'
           || maze_[current_position.row][current_position.column + 1] == '_')
            extensible = true;
    }
    return extensible;

}
Position MazeSolver::getNewPosition(Position old_position, direction dir) {
    Position newPosition;
    if(dir == SOUTH){
        newPosition.row = old_position.row + 1;
        newPosition.column = old_position.column;
    }
    else{
        newPosition.row = old_position.row;
        newPosition.column = old_position.column + 1;
    }
    return newPosition;
}

void MazeSolver::printSolution() {
    for(int i = 0;  i < maze_rows_ ; ++i){
        for(int j = 0; j < maze_columns_ ; ++j){
            std::cout << solution_[i][j]<<" " ;
        }
        std::cout <<std::endl;
    }
}