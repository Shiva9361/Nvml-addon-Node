{
    "targets": [
        {
            "target_name": "nvml",
            "sources": ["src/nvml.cpp"],
            "dependencies": [
                "<!(node -p \"require('node-addon-api').targets\"):node_addon_api",
            ],
            "libraries": [
                "-lnvidia-ml",
            ]
        }
    ]
}
