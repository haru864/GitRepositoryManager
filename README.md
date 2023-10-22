# GitRepositoryManager
Gitローカルリポジトリの状態をチェックするアプリです。<br>
アプリを起動すると、対話型インターフェースで下記の操作ができます。
* ローカルリポジトリのリストアップ
* リポジトリの状態チェック
* ターミナルへのチェック結果の表示

# 動作環境

下記と同じ、または同等の環境で動作します。
```
OS: Ubuntu 22.04.3 LTS
Linux version 6.2.0-35-generic
```

# インストール
リポジトリ内の下記ファイルをダウンロードして、任意のディレクトリに置きます。
```bash
build/GitRepositoryManager
```
同じディレクトリに以下のファイルを作成します。
```bash
tmp/check_result.json
tmp/repository_abspath_list.txt
```

# 使い方
ターミナルからGitRepositoryManagerを起動します。
```bash
./GitRepositoryManager
```

基本は、**scan** -> **check** -> **list** の順で実行します。

**help** を入力するとマニュアルが表示されます。<br>
**exit** を入力するとアプリを終了します。

![Screenshot from 2023-10-23 00-11-13](https://github.com/haru864/GitRepositoryManager/assets/45516420/bcfab839-ba41-4e72-8503-1c12128c2a7c)

**scan** でディレクトリを指定すると、そのディレクトリ配下にあるリポジトリを走査します。<br>
見つかったリポジトリは、**tmp/repository_abspath_list.txt** に絶対パスが記録されます。

![Screenshot from 2023-10-23 00-07-34](https://github.com/haru864/GitRepositoryManager/assets/45516420/034eaa14-2c01-47f5-929f-e2d8b9bccd25)

**help** を実行して、**scan** で発見したリポジトリの状態をチェックします。

![Screenshot from 2023-10-23 00-10-13](https://github.com/haru864/GitRepositoryManager/assets/45516420/fe45f9a6-5873-4b71-aa76-8cd630c83bbd)

**help** を実行して **scan** 結果を表示します。<br>
リポジトリの状態は5種類に分けられます。<br>
1. clean コミットされていない変更がなく、ローカルとリモートでコミット差分もない
2. difference_branch いずれかのブランチでローカルとリモートでコミット差分がある
3. uncommited_changes コミットされていない変更がある
4. unpushed_branch いずれかのブランチがリモートに存在しない
5. untracked_changes ステージにaddされていない変更がある

ディレクトリパスの左側に表示されている数字を入力すると、そのディレクトリを作業ディレクトリとする別のターミナルが開きます。<br>
すべてのリポジトリが clean として表示されていれば、変更内容がすべてリモートにある状態です。

![Screenshot from 2023-10-23 00-14-24](https://github.com/haru864/GitRepositoryManager/assets/45516420/afbf0a2d-d9dc-4cad-867d-cfc2c5d22c9a)
