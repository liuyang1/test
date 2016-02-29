import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdOut;
public class Solver {
    public Solver(Board initial)           // find a solution to the initial board (using the A* algorithm)
    {
    }
    public boolean isSolvable() // is the initial board solvable?
    {
        return false;
    }
    public int moves()                     // min number of moves to solve initial board; -1 if unsolvable
    {
        return -1;
    }
    public Iterable<Board> solution()      // sequence of boards in a shortest solution; null if unsolvable
    {
        return null;
    }
    // solve a slider puzzle
    public static void main(String[] args) {
        // create initial board from file
        In in = new In(args[0]);
        int N = in.readInt();
        int[][] blocks = new int[N][N];
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                blocks[i][j] = in.readInt();
        Board initial = new Board(blocks);

        // solve the puzzle
        Solver solver = new Solver(initial);

        // print solution to standard output
        if (!solver.isSolvable())
            StdOut.println("No solution possible");
        else {
            StdOut.println("Minimum number of moves = " + solver.moves());
            for (Board board : solver.solution())
                StdOut.println(board);
        }
    }
}
