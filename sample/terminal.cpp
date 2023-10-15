#include <iostream>
#include <string>
#include <cstdlib>

int main()
{
    // ユーザーからのディレクトリパスの入力を要求する
    std::string directory_path;
    std::cout << "ディレクトリのパスを入力してください: ";
    std::getline(std::cin, directory_path);

    // ディレクトリパスを使って新しいターミナルタブを開くコマンドを構築
    std::string command = "gnome-terminal --tab -- bash -c 'cd " + directory_path + "; exec bash'";

    // システムコールを使用してコマンドを実行
    std::system(command.c_str());

    return 0;
}
