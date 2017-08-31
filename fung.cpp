#include<getopt.h>
#include"fung.hpp"




namespace{


FILE*   in;
FILE*  out;

std::string  in_path;

bool  quiet;


std::string
load_file(FILE*  f)
{
  std::string  s;

    if(f)
    {
        for(;;)
        {
          auto  c = fgetc(f);

            if(feof(f))
            {
              break;
            }


            if(ferror(f))
            {
              printf("ファイル読み込み中にエラー");

              break;
            }


          s.push_back(c);
        }


      fclose(f);
    }


  return std::move(s);
}


void
process_option(int  argc, char**  argv)
{
  static struct option  const options[] = {
    {"help",no_argument,nullptr,'h'},
    {"quiet",no_argument,nullptr,'q'},
  };


    for(;;)
    {
      auto  c = getopt_long(argc,argv,"D::U:I:",options,nullptr);

        switch(c)
        {
      case('h'):
          printf("使い方: pp {オプション} 入力ファイルパス [出力ファイルパス]\n");
          printf("入力ファイルパスに \"-\" が渡された場合、標準入力から読み込む。省略できない\n");
          printf("出力ファイルパスに \"-\" か渡されるか省略された場合、標準出力へ書き込む\n");
          printf("\n");
          exit(0);
          break;
      case('q'):
          quiet = true;
          break;
      case('?'):
          break;
      case(-1):
            for(int  i = optind;  i < argc;  ++i)
            {
              std::string  arg(argv[i]);

                if(arg == "-")
                {
                    if(!in){ in =  stdin;}
                  else     {out = stdout;}
                }

              else
                {
                  auto  path = arg.data();

                    if(!in)
                    {
                      in_path = path;

                      in = fopen(path,"rb");

                        if(!in)
                        {
                          printf("入力ファイル%sを開けませんでした\n",path);

                          exit(-1);
                        }
                    }

                  else
                    {
                      out = fopen(path,"wb");

                        if(!out)
                        {
                          printf("出力ファイル%sを開けませんでした\n",path);

                          exit(-1);
                        }
                    }
                }
            }


            if(!in)
            {
              in = stdin;
            }


            if(!out)
            {
              out = stdout;
            }
          return;
        }
    }
}


}


int
main(int  argc, char**  argv)
{
  using namespace fung;

  process_option(argc,argv);


  auto  s = load_file(in);

  Cursor  cur(s.data(),new std::string(in_path));

  std::unique_ptr<GlobalSpace>  gsp;

    try
    {
      gsp = make_global_space(cur);
    }


    catch(Error&  e)
    {
      printf("ERROR\n");

      e.cursor.print();

      printf("\n%s\n",e.what());

      printf("グローバル空間の生成に失敗\n");
 
      exit(-1);
    }


    try
    {
        if(gsp)
        {
          gsp->print();

          Context  ctx(gsp);

            if(ctx.prepare_to_run("main"))
            {
                if(ctx.run())
                {
                  printf("返り値: ");

                  ctx.get_returned_value().print();
                }

              else
                {
                  printf("中断しました");
                }
            }


          printf("\n");
        }
    }


    catch(Error&  e)
    {
      printf("ERROR\n");

      e.cursor.print();

      printf("\n%s\n",e.what());

      printf("実行に失敗\n");
    }


  fclose(out);

  return 0;
}



