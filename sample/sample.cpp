#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    std::string dir_path;
    std::cout << "ディレクトリパスを入力してください: ";
    std::getline(std::cin, dir_path);

    int pipefd[2];
    // パイプを作成; pipefd[0]は読み込み用, pipefd[1]は書き込み用
    if (pipe(pipefd) == -1)
    {
        std::cerr << "パイプの作成に失敗しました。" << std::endl;
        return 1;
    }

    pid_t pid = fork();

    if (pid == -1)
    {
        std::cerr << "fork 失敗" << std::endl;
        return 1;
    }
    else if (pid > 0)
    {
        // 親プロセス
        close(pipefd[0]); // 読み込み側のエンドポイントを閉じる

        std::string input;
        while (true)
        {
            std::getline(std::cin, input);
            if (input == "exit")
            {
                break; // "exit"が入力されたら終了
            }
            input.push_back('\n');                         // 改行文字を追加
            write(pipefd[1], input.c_str(), input.size()); // パイプにデータを書き込む
        }

        close(pipefd[1]);      // 書き込み側も閉じる
        waitpid(pid, NULL, 0); // 子プロセスの終了を待つ
    }
    else
    {
        // 子プロセス
        if (chdir(dir_path.c_str()) == -1)
        {
            std::cerr << "chdir 失敗: " << dir_path << std::endl;
            exit(1);
        }

        close(pipefd[1]);              // 書き込み側のエンドポイントを閉じる
        dup2(pipefd[0], STDIN_FILENO); // 標準入力をパイプの読み込み側に置き換える

        execl("/bin/bash", "bash", (char *)NULL); // bashを実行

        // execlが失敗した場合、エラーメッセージを出力
        std::cerr << "execl 失敗" << std::endl;
        exit(1);
    }

    return 0;
}
