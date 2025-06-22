{
    "targets": [
        {
            "target_name": "js-randomness-predictor-cpp",
            "sources": [
                "<!@(ls -1 src/*.cpp)",
                "<!@(ls -1 src/wrappers/*.cpp)",
            ],
            "include_dirs": [
                "src/",
                "src/wrappers/",
                "third_party/z3/include/",
                "<!(node -p \"require('node-addon-api').include\")",
            ],
            "dependencies": [
                "<!(node -p \"require('node-addon-api').targets\"):node_addon_api_except_all",
            ],
            "cflags_cc+": ["-std=c++20"],
            "cflags!": ["-std:c++20"],
            "conditions": [
                [
                    'OS=="mac"',
                    {
                        "libraries": [
                            "<(module_root_dir)/third_party/z3/lib/darwin-<(target_arch)/libz3.a"
                        ],
                        "cflags+": ["-fvisibility=hidden"],
                        "xcode_settings": {
                            "GCC_SYMBOLS_PRIVATE_EXTERN": "YES",
                        },
                    },
                ],
                [
                    "OS=='linux'",
                    {
                        "libraries": [
                            "<(module_root_dir)/third_party/z3/lib/linux-<(target_arch)/libz3.a"
                        ]
                    },
                ],
                [
                    "OS=='win'",
                    {
                        "msvs_settings": {
                            "VCCLCompilerTool": {
                                "LanguageStandard": "stdcpp20",
                            }
                        },
                        "msbuild_settings": {
                            "ClCompile": {"LanguageStandard": "stdcpp20"}
                        },
                        "libraries": [
                            "<(module_root_dir)/third_party/z3/lib/win32-x64/libz3.lib"
                        ],
                    },
                ],
            ],
        }
    ]
}
